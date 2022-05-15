#include <bits/stdc++.h>
using namespace std;

#define MAX_TREE_HT 50

//Min heap node
struct Node {
    char c;
    uint32_t f;
    Node *leftNode;
    Node *rightNode;
};

//Structure for minheap tree
struct MinHeap {
    uint32_t capacity;
    uint32_t curSize;
    Node **arr;
};

//Global variables to store huffman code and minheap tree
std::map<char, string> Hcode;
MinHeap *gMinHeap;

//Prints and stores the huffman code
void ObtainHufCode(Node *root,string str)
{
    //If left child existes, assign 0 and traverse
    if (root->leftNode)
    {
        ObtainHufCode(root->leftNode, str+"0");
    }

    //If right child existes, assign 1 and traverse
    if (root->rightNode)
    {
        ObtainHufCode(root->rightNode,  str+"1");
    }

    // Check if the current node is leaf
    if( (root->leftNode == nullptr) && (root->rightNode == nullptr))
    {
        Hcode[root->c] = str;
        printf("\n\t%c : %s",root->c,str.c_str() );
    }
}

//Utility to create new node
Node *CreateNewNode(char c, uint32_t f)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->f = f;
    newNode->c = c;
    newNode->leftNode = NULL;
    newNode->rightNode = NULL;
    return newNode;
}

//Utility to swap 2 nodes
void SwapNodes(Node **x, Node **y)
{
    Node *t = *x;
    *x = *y;
    *y = t;
}

//Utility to min heapify the min heap tree
void Heapify(MinHeap *minHeap, int idx)
{
    int leftNode = 2 * idx + 1;
    int rightNode = 2 * idx + 2;
    int smallest = idx;

    if (leftNode < minHeap->curSize && minHeap->arr[leftNode]->f < minHeap->arr[smallest]->f)
    {
        smallest = leftNode;
    }

    if (rightNode < minHeap->curSize && minHeap->arr[rightNode]->f < minHeap->arr[smallest]->f)
    {
        smallest = rightNode;
    }

    if (smallest != idx)
    {
        SwapNodes(&minHeap->arr[smallest], &minHeap->arr[idx]);
        Heapify(minHeap, smallest);
    }
}

// Utility to extract min value from the Minheap
Node* ExtractMinElement(MinHeap *minHeap)
{
    Node *t = minHeap->arr[0];
    minHeap->arr[0] = minHeap->arr[minHeap->curSize - 1];
    --minHeap->curSize;
    Heapify(minHeap, 0);
    return t;
}

//Inserts new node to the MinHeap
void Insert(MinHeap *minHeap, Node *node)
{
    ++minHeap->curSize;
    int i = minHeap->curSize - 1;
    while (i && node->f < minHeap->arr[(i - 1) / 2]->f)
    {
        minHeap->arr[i] = minHeap->arr[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    minHeap->arr[i] = node;
}

//Utility to decode the encoded huffman string
//If s[i] is 0, then iterate to the left child aqnd if one iterate to right
string DecodeHuffmanCode(Node* root, string s)
{
    string ans = "";
    Node* curr = root;
    for (int i=0;i<s.size();i++)
    {
        if (s[i] == '0')
        {
            curr = curr->leftNode;
        }
        else
        {
            curr = curr->rightNode;
        }

        //if the current node is leaf, get the data from the node
        if (curr->leftNode==NULL && curr->rightNode==NULL)
        {
            ans += curr->c;
            curr = root;
        }
    }
    return ans+'\0';
}

//Util to construct huffman tree
Node *ConstructHuffmenTree(char c[], int f[], int curSize)
{
    Node *leftNode, *rightNode, *top;
    //Allocates minheap with size of given capacity
    gMinHeap = (MinHeap *)malloc(sizeof(MinHeap));
    gMinHeap->arr = (Node **)malloc(gMinHeap->capacity * sizeof(Node *));
    gMinHeap->capacity = curSize;
    gMinHeap->curSize = 0;

    //Creates node for given data and insert it to the MinHeap
    for (int i = 0; i < curSize; ++i)
    {
        gMinHeap->arr[i] = CreateNewNode(c[i], f[i]);
    }

    gMinHeap->curSize = curSize;


    //Heapifies the built tree
    for (int i = (curSize - 2) / 2; i >= 0; --i)
    {
        Heapify(gMinHeap, i);
    }

    //Execute until the min heap size becomes 1
    while (gMinHeap->curSize != 1)
    {
        //Get 2 min nodes from the min heap
        leftNode = ExtractMinElement(gMinHeap);
        rightNode = ExtractMinElement(gMinHeap);
        
        //Create new node with data + and frequency as sum of the 2 min nodes
        top = CreateNewNode('+', leftNode->f + rightNode->f);

        //Make these 2 nodes as left and right children
        top->leftNode = leftNode;
        top->rightNode = rightNode;

        //Inserts this internal node to the tree
        Insert(gMinHeap, top);
    }
    return top;
}

// Constructs Huffman tree and prints the huffman code
Node * HuffmanCodes(char c[], int f[], int curSize)
{
    Node *root = ConstructHuffmenTree(c, f, curSize);

    printf("\n\nHuffman Code");

    ObtainHufCode(root, "");
    return root;
}


int main()
{
    string str,encStr, decStr;
    std::map<char, int> freqMap;
    printf("\nEnter the string to perfom Huffman coding and decoding\n");
    cin >> str; 

    int n = str.length(), idx;
    char sArr[n];
    int fArr[n];
    Node * root;

    //Get the frequencies of each letter from the given input string
    for (int i=0; i<n; i++)
    {
        if(freqMap.find(str[i])!=freqMap.end())
        {
            freqMap[str[i]]++;
        }
        else
        {
            freqMap[str[i]] = 1;
        }
    }
    printf("\nFrequencies of input string %s", str.c_str());
    //Store the char and frequencies to the array and print
    for(auto i: freqMap)
    {
        sArr[idx] = i.first;
        fArr[idx] = i.second;
        printf("\n\t%c : %d", sArr[idx], fArr[idx]);
        idx++;
    }

    //Find the huffman code
    root = HuffmanCodes(sArr, fArr, freqMap.size());
    
    //Encode the given string with huffman code
    for (auto i: str)
    {
        encStr+=Hcode[i];
    }
  
    printf("\n\nEncoded input: %s", encStr.c_str());
  
    //Decode the encoded huffman string
    decStr = DecodeHuffmanCode(root, encStr);

    //Print the decoded value
    printf("\nDecoded input: %s", decStr.c_str());
    return 0;
}