#include "cb_queue_linklist.h"
#include <icall.h>


static queue_t  *queueEntity[QUEUE_INDEX_END];

//创建一个队列
bool link_queue_create(queueIndex_t index)
{
  queue_t *queue = (queue_t *)ICall_malloc(sizeof(queue_t));             //在栈再申请一个空间
  if(queue == NULL){
    return false;
  }
  
  linklist_t *head = (linklist_t *)ICall_malloc(sizeof(linklist_t));   // 队列头
  if(head == NULL){
    ICall_free(queue);
    return false;
  }
  head->next = NULL;
  
  queueEntity[index] = queue;
  
  queueEntity[index]->element = 0;
  queueEntity[index]->front = head;             // 空队列状态
  queueEntity[index]->rear = head;
  
  return true;
}

//入队（尾插法）
bool link_queue_input(queueIndex_t index,data_t value)
{
  linklist_t *node = (linklist_t *)ICall_malloc(sizeof(linklist_t));
  if(node == NULL){
    return false;
  }
  node->data = value;
  node->next = NULL;               //尾插的数据始终指向下一个为NULL的数据

  queueEntity[index]->rear->next = node;        //链接
  queueEntity[index]->rear = node;              //移动
  queueEntity[index]->element++;
  
  return true;
}

//回队（头插法）
bool  reverse_link_queue_input(queueIndex_t index,data_t value)
{
  linklist_t *node = (linklist_t *)ICall_malloc(sizeof(linklist_t));
  if(node == NULL){
    return false;
  }
  
  node->data = value;
  node->next = NULL;               //尾插的数据始终指向下一个为NULL的数据
  
  if(queueEntity[index]->rear->next == NULL){
    queueEntity[index]->rear = node;
  }
  
  node->next = queueEntity[index]->front->next;//链接
  queueEntity[index]->front->next = node;
  queueEntity[index]->element++;
  
  return true;
}

//判断队列是否为空
bool link_queue_is_empty(queueIndex_t index)
{
   return queueEntity[index]->front->next == NULL ? true: false;
}

//出队（头删法）
bool link_queue_output(queueIndex_t index, data_t  *data)
{
  if( link_queue_is_empty(index) ){
    return false;
  }
  linklist_t *temp = queueEntity[index]->front->next;
  *data = temp->data;
  
  queueEntity[index]->front->next = temp->next;
  queueEntity[index]->element--;
  ICall_free(temp);
  temp = NULL;

  if(queueEntity[index]->front->next == NULL)           //出现空队列时，需要更改queue->front的指向
  {
    queueEntity[index]->rear = queueEntity[index]->front;
  }
  
  return true;
}


data_t * searchDataFromQueue(uint8_t  sn)
{
  linklist_t *temp = queueEntity[QUEUE_INDEX_REPORT]->front;
  do{
    temp = temp->next;
    if(temp->data.queueReport.sn == sn){
      return &(temp->data);
    }
    
  }while(temp->next != NULL);
  return NULL;
}