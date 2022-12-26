#include<bits/stdc++.h>
#include <stack>
#include<stack>
#define MAX_VALUE 2147483647
 using namespace std;
 
typedef struct pairingH{
    int nodeName;
    int priorityValue;
    struct pairingH *left;
    struct pairingH *right;
    struct pairingH *child;
}PairingHeapNode;
 
PairingHeapNode* insert(int nodeName,int priorityValue,PairingHeapNode* PairingHeap);
bool isEmptyPH(PairingHeapNode* PairingHeap);
PairingHeapNode* FindMinPH(PairingHeapNode* PairingHeap);
PairingHeapNode* DeleteMinPH(PairingHeapNode* PairingHeap);
PairingHeapNode* GeneratePairingHeap(void);
PairingHeapNode* DecreaseKeyPH(int nodeName,int newPriorityValue, PairingHeapNode* PairingHeap);
 
PairingHeapNode* MakePairingHeap(int nodeName,int priorityValue);
PairingHeapNode* MergePH(PairingHeapNode*,PairingHeapNode*);
 
void CutPH(int nodeName,PairingHeapNode* PairingHeap);
void DeletePH(int nodeName, PairingHeapNode* PairingHeap);
 
map<int, PairingHeapNode*> NodeAddressMap;
stack<PairingHeapNode*> stackPH;

PairingHeapNode* GeneratePairingHeap(){
    return MakePairingHeap(MAX_VALUE,MAX_VALUE);
}
 
PairingHeapNode* MakePairingHeap(int nodeName,int priorityValue){
    PairingHeapNode *tmpPairingHeap = (PairingHeapNode*) malloc(sizeof(PairingHeapNode));
    tmpPairingHeap->nodeName = nodeName;
    tmpPairingHeap->priorityValue = priorityValue;
    tmpPairingHeap->left = NULL;
    tmpPairingHeap->right = NULL;
    tmpPairingHeap->child = NULL;
    NodeAddressMap[nodeName] = tmpPairingHeap;
    return tmpPairingHeap;
 
}
 
PairingHeapNode* MergePH(PairingHeapNode* PH1,PairingHeapNode* PH2){
 
    if(PH1 == NULL) return PH2;
    if(PH2 == NULL)return PH1;
 
    if(PH1->priorityValue <= PH2->priorityValue){
        if(PH1->child != NULL){
            PH1->child->left = PH2;
        }
        PH2->right = PH1->child;
        PH2->left = PH1;
        PH1->child = PH2;
 
        return PH1;
    }else{
        if(PH2->child != NULL){
            PH2->child->left = PH1;
        }
        PH1->right = PH2->child;
        PH1->left = PH2;
        PH2->child = PH1;
        return PH2;
    }
}
 
PairingHeapNode* insert(int nodeName,int priorityValue,PairingHeapNode* PairingHeap){
    PairingHeapNode* tmpPH = MakePairingHeap(nodeName,priorityValue);
    return MergePH(tmpPH,PairingHeap);
}
 
PairingHeapNode* FindMinPH(PairingHeapNode* PairingHeap){
    return PairingHeap;
}
 
bool isEmptyPH(PairingHeapNode* PairingHeap){
    if(PairingHeap == NULL) return true;
    return false;
}
 
PairingHeapNode* TwoPassPairing(PairingHeapNode* PairingHeap){
    PairingHeapNode* tmpHeap1;
    PairingHeapNode* tmpHeap2;
    PairingHeapNode* result=NULL;
 
    while(PairingHeap != NULL){
        tmpHeap1 = PairingHeap;
        tmpHeap2 = PairingHeap->right;
        if(tmpHeap2!=NULL){
            PairingHeap = tmpHeap2->right;
        }
        stackPH.push(MergePH(tmpHeap1,tmpHeap2));
 
        if(tmpHeap2==NULL){
            break;
        }
    }
 
    if(!stackPH.empty()){
        tmpHeap1 = stackPH.top();
        stackPH.pop();
        while(!stackPH.empty()){
            tmpHeap2 = stackPH.top();
            stackPH.pop();
            tmpHeap1 = MergePH(tmpHeap1,tmpHeap2);
        }
        result = tmpHeap1;
    }
    return result;
}
 
PairingHeapNode* DeleteMinPH(PairingHeapNode* PairingHeap){
    if(PairingHeap != NULL){
        PairingHeapNode* mTmp = PairingHeap->child;
        free(PairingHeap);
        mTmp = TwoPassPairing(mTmp);
        if((mTmp != NULL) && (mTmp->priorityValue == MAX_VALUE)){
            return NULL;
        }else{
            return mTmp;
        }
    }else{
        return NULL;
    }
}
 
PairingHeapNode* DecreaseKeyPH(int nodeName,int newPriorityValue, PairingHeapNode* PairingHeap){
    PairingHeapNode* decreaseNode = NodeAddressMap[nodeName];
    decreaseNode->priorityValue = newPriorityValue;
    PairingHeapNode* tmp = NULL;
    PairingHeapNode* parent= NULL;
    tmp = decreaseNode;
    while(1){
        if(tmp->left!=NULL){
            if(tmp->left->right == tmp){
                tmp = tmp->left;
                continue;
            }else{
                parent = tmp->left;
                if(parent->priorityValue > newPriorityValue){
                    parent->child = NULL;
                    tmp->left = NULL;
                    tmp = TwoPassPairing(tmp);
                    tmp = MergePH(tmp, PairingHeap);
                    return tmp;
                }
            }
        }
        break;
    }
    return PairingHeap;
}
 

main()
{
    freopen("input.txt","r",stdin);
    int i;
    PairingHeapNode *pHN = GeneratePairingHeap();
    while(1){
        cin>>i;
        if(i == -1){
            break;
        }
        pHN = insert(i,i,pHN);
    }
    cout << "After: "<<endl;
    pHN = DecreaseKeyPH(100,1,pHN);
    pHN = DecreaseKeyPH(3,200,pHN);
   while(!isEmptyPH(pHN)){
        pHN = FindMinPH(pHN);
        cout<<"Now Parent: "<<pHN<<":: Name: "<<pHN->nodeName<< ",Value:"<<pHN->priorityValue<<endl;
		pHN = DeleteMinPH(pHN);
    }
    return 0;
}
