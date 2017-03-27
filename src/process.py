

def loadTxt(file_path):
    f = open(file_path, 'r')
    papers = [];
    paper_cnt = 0;
    for line in f:
        line = line.strip();
        try:
            if line == "":
                continue
            if line[0] != '#':
                paper_num = int(line)
            elif line[0] == '#':
                if line[1] == '*':
                    papers.append({'name':line[2:]})
                elif line[1] == '@':
                    papers[paper_cnt]['author'] = line[2:].strip().split(',')
                elif line[1] == 'y':
                    papers[paper_cnt]['year'] = int(line[5:])
                elif line[1] == 'c':
                    if line[2] == 'o':
                        papers[paper_cnt]['conf'] = line[5:]
                    elif line[2] == 'i':
                        papers[paper_cnt]['citation'] = int(line[9:])
                elif line[1] == 'i':
                    papers[paper_cnt]['index'] = int(line[6:])
                elif line[1] == 'a':
                    papers[paper_cnt]['arnetid'] = int(line[8:])
                    paper_cnt += 1
        except ValueError:
            print line
    f.close();
    return papers

def loadGraph(file_path):
    f = open(file_path, 'r');
    v_e_size = f.readline().strip().split()
    vertex_size, edge_size = int(v_e_size[0]), int(v_e_size[1])
    coauthor_graph = {}
    for line in f.xreadlines():
        a1_a2_cnt = line.strip().split()
        author1, author2, ref_cnt = map(int, a1_a2_cnt.split())
        if author1 not in coauthor_graph:
            coauthor_graph[author1] = {}
        coauthor_graph[author1][author2] = cnt
    return coauthor_graph


def splitDataSet(dataset, tag):
    taglist = ['name', 'yaer', 'conf', 'citation', 'index', 'arnetid'];
    if tag not in taglist:
        print 'Tag only for: ' + str(taglist)
        return
    sections = {}
    for paper in dataset:
        if paper[tag] not in sections:
            sections[paper[tag]] = []
        sections[paper[tag]].append(paper)
    return sections

def genCoauthorGraph(dataset):
    author_cnt = 0
    link_cnt = 0
    author_dict = {}
    index_dict = {}
    author_graph = {}
    for paper in dataset:
        author_list = sorted(paper['author'])
        if author_list[0] == '':
            author_list = author_list[1:]
        for i in range(len(author_list)):
            author = author_list[i].strip()
            if author not in author_dict:
                author_dict[author] = author_cnt
                index_dict[author_cnt] = author
                author_cnt += 1
                author_graph[author] = {}
            if i != len(author_list)-1:
                for coauthor in author_list[(i+1):]:
                    coauthor = coauthor.strip()
                    if coauthor not in author_dict:
                        author_dict[coauthor] = author_cnt
                        index_dict[author_cnt] = coauthor
                        author_cnt += 1
                        author_graph[coauthor] = {}
                    if coauthor not in author_graph[author]:
                        author_graph[author][coauthor] = 1
                        link_cnt += 1
                    else:
                        author_graph[author][coauthor] += 1
    return author_cnt, link_cnt, author_dict, index_dict, author_graph


def genTwoConfAuthorDataset(dataset, conf1, conf2):
    import copy
    sections = {conf1:[], conf2:[]}
    for paper in dataset:
        if paper['conf'] in [conf1, conf2]:
            conf = paper['conf']
            paper_copy = copy.copy(paper)
            paper_copy['author'] = [conf1+'&'+author for author in paper['author']]
            sections[conf].append(paper_copy)
    return paper_copy


def getIdentityResolutionRecall(nodevec, index_dict, total_cnt, topk):
    hit_cnt = 0

    for vec in nodevec.vocab.keys():
        simlist = [item[0] for item in nodevec.most_similar(vec, topn=topk)]
        (conf1,author1) = index_dict[vec].split('&')
        for item in aimlist:
            (conf2, author2) = index_dict[item].split('&')
            if conf2 != conf1 and author1 == author2:
                hit_cnt += 1
                break
    return hit_cnt/total_cnt

def getIdentityResolutionRecall1(read_path, query_path, data_path):
    rf = open(read_path, 'r')
    qf = open(query_path, 'r')
    df = open(data_path, 'r')
    query_dict = {}
    data_dict = {}
    hit_cnt = 0
    total_cnt = 0
    for line in qf:
        line = line.strip().split(':')
        query_dict[line[1].strip()] = line[0].strip()
    for line in df:
        line = line.strip().split(':')
        if line[0].strip() in query_dict.values():
            data_dict[line[1].strip()] = line[0].strip()
            total_cnt += 1
            
    for line in rf:
        line = line.strip().split(' ')
        qauthor = query_dict[line[0].strip()]
        if qauthor in [data_dict.get(pair.split(':')[0], 'unknown') for pair in line[1:]]:
            hit_cnt += 1
    rf.close()
    df.close()
    qf.close()
    return hit_cnt, total_cnt, float(hit_cnt)/total_cnt
        



def saveCoauthorGraph(coauthor_info, file_head):
    author_cnt = coauthor_info[0]
    link_cnt = coauthor_info[1]
    author_dict = coauthor_info[2]
    author_graph = coauthor_info[4]
    f = open(file_head+'.graph', 'w')
    f.write(str(author_cnt)+' '+str(link_cnt)+'\n')
    
    for author in author_dict:
        for coauthor in author_graph[author]:
            f.write(' '.join([str(author_dict[author]), \
                    str(author_dict[coauthor]), \
                    str(author_graph[author][coauthor])])+'\n')
    f.close()
    fdict = open(file_head+'.dict', 'w')
    for author,index in author_dict.items():
        fdict.write(author+':'+str(index)+'\n')
    fdict.close()
 
def saveDataSet(dataset, file_path):
    import pandas as pd
    df = pd.DataFrame(dataset)
    df.to_csv(file_path, index=False)



