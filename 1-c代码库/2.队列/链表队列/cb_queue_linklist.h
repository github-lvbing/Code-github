//队列-链表
#ifndef _QUEUE_LINKLIST_H_
#define _QUEUE_LINKLIST_H_

#include <stdio.h> 
#include <stdlib.h>
#include "stdbool.h"
#include "cb_protocol_report.h"


typedef  enum {
  QUEUE_INDEX_REPORT = 0,
  QUEUE_TINDEX_REQUEST,
  QUEUE_INDEX_END,
}queueIndex_t;

//QUEUE_INDEX_REPORT
typedef struct {
  uint8_t sn;
  reportMethodsIndex_t HnadleIndex;
  int count;    
}queueReport_t;

//QUEUE_TINDEX_REQUEST
typedef struct {
  uint8_t cmd;
  uint8_t sn;
  uint16_t dataLen;
  uint8_t data[DATALEN];
}queueRequest_t;

typedef  union{
  queueReport_t  queueReport;
  queueRequest_t  queueRequest;
}data_t;

//队列节点类型定义
typedef  struct linknode
{
  data_t data;
  struct linknode *next;
}linklist_t;

//队列定义
typedef struct 
{
  linklist_t *front;                  //始终指向队列的头节点
  linklist_t *rear;                   //始终指向队列的尾节点，跟随入队而移动
  uint8_t  element;
}queue_t;


//接口声明
extern bool link_queue_create(queueIndex_t index);
extern bool link_queue_input(queueIndex_t index,data_t value);
extern bool link_queue_output(queueIndex_t index, data_t  *data);
extern bool reverse_link_queue_input(queueIndex_t index,data_t value);
extern data_t * searchDataFromQueue(uint8_t  sn);

#endif