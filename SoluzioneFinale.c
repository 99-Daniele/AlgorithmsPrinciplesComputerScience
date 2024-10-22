#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct graphWeight{
	long long index;
	long long weight;
};

struct source{
	char* vectors;
	long long weight;
};

struct source* newGraph;
struct graphWeight* topGraphs;
long long lenght = 0;

char* nextNumber(char* punt, long long* index, long long totalNodes);
struct source* updateWeihts(struct source* graph, long long* idx, long long totalNodes, int* reach);
long long minDistance(struct source* graph, long long *idx, long long totalNodes, int* reach, int last);
long long minGraphWeight(struct source* graph, long long totalNodes, long long topK);
struct graphWeight* swap(struct graphWeight* topGraphs, long long index1, long long index2);
struct graphWeight* heapSortFromFirst(struct graphWeight* topGraphs, long long index); 
struct graphWeight* buildHeap(struct graphWeight* topGraphs);
struct graphWeight* sum(struct source* graph, struct graphWeight* topGraphs, long long totalNodes, long long index, long long topK);
struct source* createRow(struct source* source, long long index, long long lenght);
void printTop(struct graphWeight* topGraphs);
void empty(long long totalNodes);

char* nextNumber(char* punt, long long* index, long long totalNodes){
    *index += 1;
    while(*punt && *index < totalNodes){
	    if(',' == *punt++){
	        if(newGraph[*index].weight != 0){
	            return punt;
	        }
	        else
	            *index += 1;
	    }
	}
	return punt;
}

struct source* updateWeights(struct source* graph, long long* idx, long long totalNodes, int* reach){
	char* punt = graph[*idx].vectors;
	long long weight = graph[*idx].weight;
	long long newWeight = -1;
	long long j = 0;
	long long* i = &j;
	long long index = *idx;
	long long curWeight = strtoul(punt, &punt, 10);
	*reach = 1;
	graph[index].weight = 0;
	while(*punt){	
			punt = nextNumber(punt, i, totalNodes);
			if(*i == totalNodes)
			    break;
			curWeight = strtoul(punt, &punt, 10);
			if(*i != index){
				if(curWeight > 0 && (graph[*i].weight == -1 || graph[*i].weight > curWeight + weight)){
					graph[*i].weight = curWeight + weight;
				}
				if(graph[*i].weight == -1)
					*reach = 0;
				if((newWeight == -1 || graph[*i].weight < newWeight) && graph[*i].weight > 0 && index != *i){
		   			*idx = *i;
		   			newWeight = graph[*i].weight;
		   		}
			}
	}
	return graph;
}

long long minDistance(struct source* graph, long long* idx, long long totalNodes, int* reach, int last){
	long long weight = graph[*idx].weight;
	if(last == 0)
		graph = updateWeights(graph, idx, totalNodes, reach);
	return weight;
}

long long minGraphWeight(struct source* graph, long long totalNodes, long long topK){
	long long weight = 0;
	long long i, index = 0;
	long long* idx = &index;
	int r = 0;
	int* reach = &r;
	for(i = 0; i < totalNodes; i++){
		long long oldIndex = *idx;
		long long newWeight;
		if(i < totalNodes - 1)
			newWeight = minDistance(graph, idx, totalNodes, reach, 0);
		else
			newWeight = minDistance(graph, idx, totalNodes, reach, 1);
		weight += newWeight;
		long long minWeight = weight + (newWeight * (totalNodes - i - 1));
		if(lenght == topK && minWeight >= topGraphs[0].weight && *reach == 1)
			return minWeight;
		if(*idx == oldIndex || (lenght == topK && weight >= topGraphs[0].weight))
			return weight;
	}
	return weight;	
}

struct graphWeight* swap(struct graphWeight* topGraphs, long long index1, long long index2){
	struct graphWeight temp = topGraphs[index1];
	topGraphs[index1] = topGraphs[index2];
	topGraphs[index2] = temp;
	return topGraphs;
}

struct graphWeight* heapSortFromFirst(struct graphWeight* topGraphs, long long index){
	long long left = index*2 + 1;
	long long right = index*2 +2;
	if((left < lenght && topGraphs[index].weight < topGraphs[left].weight) || (right < lenght && topGraphs[index].weight < topGraphs[right].weight)){
		if(right >= lenght ||  topGraphs[left].weight >= topGraphs[right].weight){
			topGraphs = swap(topGraphs, index, left);
			return heapSortFromFirst(topGraphs, left);
		}
		else{
			topGraphs = swap(topGraphs, index, right);
			return heapSortFromFirst(topGraphs, right);
		}	
	}
	else
		return topGraphs;
}

struct graphWeight* buildHeap(struct graphWeight* topGraphs){
	long long i;
	for(i = lenght/2; i >= 0; i--)
		topGraphs = heapSortFromFirst(topGraphs, i);
	return topGraphs;	
}

struct graphWeight* sum(struct source* graph, struct graphWeight* topGraphs, long long totalNodes, long long index, long long topK){
	if(lenght == topK && topGraphs[0].weight == 0)
		return topGraphs;
	long long sumWeight = minGraphWeight(graph, totalNodes, topK);
	if(lenght < topK){
		topGraphs[lenght].weight = sumWeight;
		topGraphs[lenght].index = index;
		lenght++;
		if(lenght == topK)
			topGraphs = buildHeap(topGraphs);
		return topGraphs;
	}
	else if(topGraphs[0].weight > sumWeight){
		topGraphs[0].weight = sumWeight;
		topGraphs[0].index = index;
		return heapSortFromFirst(topGraphs, 0);
	}
	else
	    return topGraphs;
}

struct source* createRow(struct source* source, long long index, long long lenght){
	if(source[index].vectors == NULL)
	    source[index].vectors = (char*)malloc(sizeof(char) * 4096);
	if(index == 0)	
	    source[0].weight = 0;
	else
	    source[index].weight = -1;	
	if(scanf("%s", source[index].vectors) == 0)
		exit(0);
	return source;
}

void printTop(struct graphWeight* topGraphs){
	long long i;
	for(i = 0; i < lenght-1; i++){
		printf("%lli ", topGraphs[i].index);
	}
	if(lenght > 0)
		printf("%lli", topGraphs[i].index);
	printf("\n");    
}

void empty(long long totalNodes){
	long long i;
	for(i = 0; i < totalNodes; i++){
		free(newGraph[i].vectors);
		newGraph[i].vectors = NULL;
	}
	free(newGraph);
	newGraph = NULL;
	free(topGraphs);
	topGraphs = NULL;	
}

int main(){
	long long totalNodes;
	long long topK;
    long long index = 0;
    long long i;
	char command[16];
	char blank;
	if(scanf("%lli%c%lli",&totalNodes, &blank, &topK) == 0)
		exit(0);
	topGraphs = (struct graphWeight*)malloc(sizeof(struct graphWeight) * topK);
	for(i = 0; i < topK; i++){
		topGraphs[i].index = 0;
		topGraphs[i].weight = 0;
	}
	newGraph = (struct source*)malloc(sizeof(struct source) * totalNodes);
	for(i = 0; i < totalNodes; i++){
		newGraph[i].vectors = NULL;
		newGraph[i].weight = 0;
	}
	while(scanf("%s", command) != EOF){
		while(strcmp("TopK", command) != 0){
			for(i = 0; i < totalNodes; i++){
				newGraph = createRow(newGraph, i, totalNodes);				
			}
			topGraphs = sum(newGraph, topGraphs, totalNodes, index, topK);
			index++;
			if(scanf("%s", command) == 0)
				exit(0);
		}
		printTop(topGraphs);
	}
	empty(totalNodes);
	return 0;
}
