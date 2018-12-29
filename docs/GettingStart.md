## **Quick start**

### **Index Build**
 ```bash
 Usage:
 ./IndexBuiler [options]
 Options:
  -d, --dimension <value>       Dimension of vector, required.
  -v, --vectortype <value>      Input vector data type (e.g. Float, Int8, Int16), required.
  -i, --input <value>           Input raw data, required.
  -o, --outputfolder <value>    Output folder, required.
  -a, --algo <value>            Index Algorithm type, required.

  -t, --thread <value>          Thread Number, default is 32.
  --delimiter <value>           Vector delimiter, default is |.
  Index.<ArgName>=<ArgValue>    Set the algorithm parameter ArgName with value ArgValue.
  ```

  ### **Index Search**
  ```bash
  Usage:
  ./Search <index folder> [options]
  Options
   Index.QueryFile=XXX           Input Query file
   Index.ResultFile=XXX	       Output result file
   Index.TruthFile=XXX           Truth file that can help to calculate the recall
   Index.K=XXX                   How many nearest neighbors return
   Index.MaxCheck=XXX            The maxcheck of the search
  ```

### **Server**
```bash
Usage:
./Server [options]
Options: 
  -m, --mode <value>              Service mode, interactive or socket.
  -c, --config <value>            Configure file of the index

Write a server configuration file service.ini as follows:

[Service]
ListenAddr=0.0.0.0
ListenPort=8000
ThreadNumber=8
SocketThreadNumber=8

[QueryConfig]
DefaultMaxResultNumber=6
DefaultSeparator=|

[Index]
List=BKT

[Index_BKT]
IndexFolder=BKT_gist
```

### **Client**
```bash
Usage:
./Client [options]
Options:
-s, --server                       Server address
-p, --port                         Server port
-t,                                Search timeout
-cth,                              Client Thread Number
-sth                               Socket Thread Number
```

### **Aggregator**
```bash
Usage:
./Aggregator

Write Aggregator.ini as follows:

[Service]
ListenAddr=0.0.0.0
ListenPort=8100
ThreadNumber=8
SocketThreadNumber=8

[Servers]
Number=2

[Server_0]
Address=127.0.0.1
Port=8000

[Server_1]
Address=127.0.0.1
Port=8010
```

### **Python Support**
> Singlebox PythonWrapper
 ```python
 
import SPTAG
import numpy as np

n = 100
k = 3
r = 3

def testBuild(algo, distmethod, x, out):
    i = SPTAG.AnnIndex(algo, 'Float', x.shape[1])
    i.SetBuildParam("NumberOfThreads", '4')
    i.SetBuildParam("DistCalcMethod", distmethod)
    ret = i.Build(x.tobytes(), x.shape[0])
    i.Save(out)

def testBuildWithMetaData(algo, distmethod, x, s, out):
    i = SPTAG.AnnIndex(algo, 'Float', x.shape[1])
    i.SetBuildParam("NumberOfThreads", '4')
    i.SetBuildParam("DistCalcMethod", distmethod)
    if i.BuildWithMetaData(x.tobytes(), s, x.shape[0]):
        i.Save(out)

def testSearch(index, q, k):
    j = SPTAG.AnnIndex.Load(index)
    for t in range(q.shape[0]):
        result = j.Search(q[t].tobytes(), k)
        print (result[0]) # ids
        print (result[1]) # distances

def testSearchWithMetaData(index, q, k):
    j = SPTAG.AnnIndex.Load(index)
    j.SetSearchParam("MaxCheck", '1024')
    for t in range(q.shape[0]):
        result = j.SearchWithMetaData(q[t].tobytes(), k)
        print (result[0]) # ids
        print (result[1]) # distances
        print (result[2]) # metadata

def testAdd(index, x, out, algo, distmethod):
    if index != None:
        i = SPTAG.AnnIndex.Load(index)
    else:
        i = SPTAG.AnnIndex(algo, 'Float', x.shape[1])
    i.SetBuildParam("NumberOfThreads", '4')
    i.SetBuildParam("DistCalcMethod", distmethod)
    if i.Add(x.tobytes(), x.shape[0]):
        i.Save(out)

def testAddWithMetaData(index, x, s, out, algo, distmethod):
    if index != None:
        i = SPTAG.AnnIndex.Load(index)
    else:
        i = SPTAG.AnnIndex(algo, 'Float', x.shape[1])
    i = SPTAG.AnnIndex(algo, 'Float', x.shape[1])
    i.SetBuildParam("NumberOfThreads", '4')
    i.SetBuildParam("DistCalcMethod", distmethod)
    if i.AddWithMetaData(x.tobytes(), s, x.shape[0]):
        i.Save(out)

def testDelete(index, x, out):
    i = SPTAG.AnnIndex.Load(index)
    ret = i.Delete(x.tobytes(), x.shape[0])
    print (ret)
    i.Save(out)
    
def Test(algo, distmethod):
    x = np.ones((n, 10), dtype=np.float32) * np.reshape(np.arange(n, dtype=np.float32), (n, 1))
    q = np.ones((r, 10), dtype=np.float32) * np.reshape(np.arange(r, dtype=np.float32), (r, 1)) * 2
    m = ''
    for i in range(n):
        m += str(i) + '\n'

    print ("Build.............................")
    testBuild(algo, distmethod, x, 'testindices')
    testSearch('testindices', q, k)
    print ("Add.............................")
    testAdd('testindices', x, 'testindices', algo, distmethod)
    testSearch('testindices', q, k)
    print ("Delete.............................")
    testDelete('testindices', q, 'testindices')
    testSearch('testindices', q, k)

    print ("AddWithMetaData.............................")
    testAddWithMetaData(None, x, m, 'testindices', algo, distmethod)
    print ("Delete.............................")
    testSearchWithMetaData('testindices', q, k)
    testDelete('testindices', q, 'testindices')
    testSearchWithMetaData('testindices', q, k)

if __name__ == '__main__':
    Test('BKT', 'L2')
    Test('KDT', 'L2')

 ```

 > Python Client Wrapper, Surpose there is a sever run at 127.0.0.1:8000 serving nytimes datasets:
 ```python
import SPTAGClient
import numpy as np
import time

def testSPTAGClient():
    index = SPTAGClient.AnnClient('127.0.0.1', '8100')
    while not index.IsConnected():
        time.sleep(1)
    index.SetTimeoutMilliseconds(18000)

    q = np.ones((10, 10), dtype=np.float32)
    for t in range(q.shape[0]):
        result = index.Search(q[t].tobytes(), 6, 'Float', False)
        print (result[0])
        print (result[1])

if __name__ == '__main__':
    testSPTAGClient()

 ```
  
  
  