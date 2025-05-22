#include"../include/utils/algorithms.hpp"
#include<stack>
#include<iostream>
#include<unordered_map>

void topological_sort(Graph &graph)
{
    //organize into layers later
    size_t num_of_nodes = graph.getNumOfNodes();
    bool* visited = new bool[num_of_nodes]();

    //find way to link nodes to vistied.
    for(size_t index; index < num_of_nodes; index++){
        Node* current_node = graph.getNodes()[index];
        dfs(visited, current_node, index);
    }

    delete[] visited;
}


void layerize(Graph &graph, std::vector<size_t> *layer_sizes)
{
    //not really needed right now
}



void inference(Graph &graph, std::vector<Tensor> &inputs)
{
    //tracking which tensor is being used;
    size_t current_tensor = 0;

    //associating nodes with their proper values via a map
    std::unordered_map<Node*, Tensor> map;
    map.reserve(graph.getNumOfNodes());
    
    std::vector<Node*> nodes = graph.getNodes();
    //add inputs
    size_t current = 0;
   
    //setting input values
    for(auto& input_index : graph.getInputNodes()){
        map[nodes[input_index]] = inputs[current];
        current++;
    }

    for(auto& node : nodes){
        //ensuring its an operation node
        if (node == nullptr) {
            std::cerr << "Null node encountered!\n";
            return;
        }
        
        if(!node->input.empty()){
            //is an input value
            Tensor result;
            if(node->input.size() < 2){
                //fix: add dest and input
                //take prev node input
                result = map[node->input[0]];
              
            }
            else{
                //currently assuming max two inputs, and the first input is first (matters for matrix mult)
                
                Tensor input1 = map[node->input[0]];
                Tensor input2 = map[node->input[1]];

                //do operation
                node->op->operate(input1, input2, result);

                std::cout << "input 1 : \n";
                input1.print();
                std::cout << "\ninput 2 : " << std::endl;
                input2.print();
                std::cout << "\napplying : " << node->name << "\nresult : " << std::endl;
                result.print();
                std::cout << "\n---------------\n---------------\n" << std::endl;
            
            }
            //mapping node to result
            map[node] = result;
        }
    }
}



void dfs(bool *visted, Node* node, size_t index)
{
    visted[index] = true;

}