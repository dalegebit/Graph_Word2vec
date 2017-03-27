# Graph Word2vec
Applying word2vec to sampled sequences from a graph.

---

## Detail

The sampling procedure is implemented in c++ with pthread, providing python wrapper, so it would be fast.


---
## How to run?

`$ cd $YOUR_PATH/src`

`$ mkdir data`

First you must install the V6 dataset from https://cn.aminer.org/citation and unzip it into *$YOUR_PATH/src/data*.
The description of the dataset is also shown on the website, which is neccesary to look at.

To compile the code, <u>boost python</u> library is needed. Once you install the library, just make it. Then you will get <u>randpaths.so</u>:

`$ make`

To train node vector, python library <u>gensim</u> is needed. And then opne python:

> import randpaths

> import process

> from gensim.models import Word2Vec

To load the dataset from:

> dataset = process.loadTxt('data/$DATA_NAME')

To split the dataset according to the tag(e.g. conference, year) :

> sections = process.splitDataSet(dataset, tag)

To generate a co-author dataset of two conferences (each author's name was added a prefix: <u>conference_name+'&'</u>, to separate the same author from different conferences):

> codataset = process.genTwoConfAuthorDataset(dataset, conf1, conf2)

To generate the co-author graph, along with author2index and index2author dict, and save(given the graph name, all graph information will be automatically saved in <u>$YOUR_PATH/src/data</u>):

> [node_num, edge_num, author2index, index2author, graph] =
process.genCoauthorGraph(codataset)

> process.saveCoauthorGraph([node_num, edge_num, author2index, index2author, graph], graph_name)

To produce random paths with c++ implemented parallel bias random path sampling procedure:

> paths = randpaths.damn_get_randpaths(saved_graph_name, T, drop_rate, epsilon, thread_num)

> paths = [path.split() for path in paths]

To train node vector from co-author dataset
(for more details: http://radimrehurek.com/gensim/models/word2vec.html):

> node2vec = Word2Vec(paths, size=dimesion, windows=wsize)

To get recall score of identity resolution(<u>total_cnt</u> is the total number of author that appears in two confereces):

> recall = process.getIdentityResolutionRecall(node2vec, index2author, total_cnt, topk)
