/*****************************************************************************
* QHsmTst example
* Last Updated for Version: 4.0.00
* Date of the Last Update:  Apr 07, 2008
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2008 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GPL and are specifically designed for licensees interested in
* retaining the proprietary status of their code.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qpn_port.h"
#include "qhsmtst.h"
#include <stdio.h>
#include <stdlib.h>
#include "elev.h"
#define UP 1
#define DOWN 2
#define STOP 1
#define NOSTOP 0
#define MOVE_TIME 5

/* QHsmTst class -----------------------------------------------------------*/
typedef struct QHsmTstTag {
    QHsm super;                                        /* extend class QHsm */
                                             /* extended state variables... */
    uint8_t foo;
} QHsmTst;

unsigned int psim = 0;
unsigned int pele = 0;

unsigned int pout = 0;
unsigned int curr = 1;
unsigned long simTime = 0;
unsigned int countSec = 0;
unsigned int EleDir = 0;


void   QHsmTst_ctor(void);                               /* the ctor */
static QState QHsmTst_initial(QHsmTst *me);  /* initial pseudostate-handler */
static QState QHsmTst_moving(QHsmTst *me);  /* initial pseudostate-handler */
static QState QHsmTst_idle    (QHsmTst *me);                  /* state-handler */
//static QState QHsmTst_approx   (QHsmTst *me);                  /* state-handler */
static QState QHsmTst_stop  (QHsmTst *me);                  /* state-handler */



struct queue{
    int ele[6];
    int rear;
};

struct queue qin;
struct queue qout;

int inQueue(struct queue q, int val){
    int i;
    for (i = 0; i < q.rear; i++) {
        if (q.ele[i] == val)
            return 1;
    }
    return 0;
}

int enQueue(struct queue *q, int val){
    if (q->rear == 5)
        return 1;

    if (!inQueue(*q, val)){
        q->ele[q->rear] = val;
        q->rear++;
    }
    return 0;
}


int deQueue(struct queue *q) {
    if (q->rear == 0)
        return 0;

    int temp = q->ele[0];
    int i;

    for (i = 0; i<q->rear; i++){
        q->ele[i] = q->ele[i+1];
    }

    q->rear--;
    return temp;
}

int isEmptyQueue(struct queue q){
    return (q.rear == 0);
}

int remEleQueue(struct queue *q, int val) {
    int i;
    for (i = 0; i < q->rear; i++){
        if (q->ele[i] == val){
            int j;
            for (j = i; j < q->rear; j++){
                q->ele[j] = q->ele[j+1];
            }
            q->rear--;
            break;
        }
    }
    return 0;
}

int headQueue(struct queue q){
    if (q.rear>0)
        return q.ele[0];
    return 0;
}

int lenQueue(struct queue q){
    return q.rear;
}
/*
int checkUpDown(struct queue *q, int dest){
    if (curr != dest){
        EleDir = (curr - dest) > 0 ? UP : DOWN;
        if (!inQueue(*q, dest)) {
            enQueue(q, dest );
            pout++;
            psim++;
            return 1;
        }
        else{
            pout++;
            psim++;
            return 1;

        }
}
    pele++;
    psim++;
    return 0;
}
*/
int decideDir(unsigned int curr, unsigned int EleDir){
    int i;
    printf("curr %d, dir %d\n", curr, EleDir);
    if (curr == 1 && EleDir == DOWN) {
        return UP;
    }

    else if (curr == 5 && EleDir == UP){
        return DOWN;
    }

    if (EleDir == UP){
        if(!isEmptyQueue(qout)){
            for (i = 0; i < qout.rear; i++){
                if(qout.ele[i] > curr)
                    return UP;
            }
        }
        printf("cp decide qout finished\n");
        if(!isEmptyQueue(qin)){
            for (i = 0; i < qin.rear; i++){
                if(qin.ele[i] > curr)
                    return UP;
            }
        }
        printf("cp decide qin\n");
        return DOWN;
    }
    else if (EleDir == DOWN){
        if(!isEmptyQueue(qout)){
            for (i = 0; i < qout.rear; i++){
                if(qout.ele[i] < curr)
                    return DOWN;
            }
        }
        printf("cp2 decide qout finished\n");
        if(!isEmptyQueue(qin)){
            for (i = 0; i < qin.rear; i++){
                if(qin.ele[i] < curr)
                    return DOWN;
            }
        }
        printf("cp2 decide qin\n");
        return UP;
    }

    return EleDir;
}

    /*
    printf("cpdeci\n");
    if (!isEmptyQueue(qout)){
        printf("cp decide qout\n");
        if (EleDir == UP){
            for (i = 0; i < qout.rear; i++){
                if(qout.ele[i] > curr)
                    return UP;
            }
            printf("cp decide qout finished\n");
            for (i = 0; i < qin.rear; i++){
                if(qin.ele[i] > curr)
                    return UP;
            }
            printf("cp decide qin\n");
            return DOWN;
        }
        else if (EleDir == DOWN){
            for (i = 0; i < qout.rear; i++){
                if(qout.ele[i] < curr)
                    return DOWN;
            }
            printf("cp decide qout finished\n");
            for (i = 0; i < qin.rear; i++){
                if(qin.ele[i] < curr)
                    return DOWN;
            }
            printf("cp decide qin\n");
            return UP;
        }
    }

    if (!isEmptyQueue(qin)){
        if (EleDir == DOWN){
            for (i = 0; i < qout.rear; i++){
                if(qout.ele[i] < curr)
                    return DOWN;
            }
            for (i = 0; i < qin.rear; i++){
                if(qin.ele[i] < curr)
                    return DOWN;
            }
            return UP;
        }
    }
*/

int displayQueue(struct queue q){
    int i;
    for (i = 0;  i < q.rear; i++)
        printf("%d ", q.ele[i]);
    printf("\n");
}

int decideStop(unsigned int currVal, unsigned int EleDir){


        printf("EleDir %d, curr %d \n", EleDir, currVal);
        printf("qin: "); displayQueue(qin);
        printf("qout: "); displayQueue(qout);
        EleDir = decideDir(currVal, EleDir);
        printf("curr prev %d\n", curr);
        curr = currVal + ((EleDir == UP) ? 1 : ((EleDir == DOWN) ? -1 : 0));
        printf("EleDir %d, curr %d \n", EleDir, curr);
        printf("current floor %d\n",curr);

        printf("qin: "); displayQueue(qin);
        printf("qout: "); displayQueue(qout);
        if(inQueue(qin, curr)){
            return STOP;
        }
        else if (inQueue(qout, curr)){
            return STOP;
        }
        printf("cp3\n");
        return NOSTOP;


        //return Q_TRAN(&QHsmTst_approx);
}
/* global objects ----------------------------------------------------------*/
QHsmTst HSM_QHsmTst;                /* the sole instance of the QHsmTst HSM */

/*..........................................................................*/
void QHsmTst_ctor(void) {
    QHsm_ctor(&HSM_QHsmTst.super, (QStateHandler)&QHsmTst_initial);
    HSM_QHsmTst.foo = 0;              /* initialize extended state variable */
}

/*..........................................................................*/
QState QHsmTst_initial(QHsmTst *me) {
    psim = 0;
    pele = 0;
    pout = 0;
    curr = 1;
    //simTime = 0;
    EleDir = UP;
    qin.rear = 0;
    qout.rear = 0;


    //BSP_display("top-INIT;");
    return Q_TRAN(&QHsmTst_idle);
}
/*..........................................................................*/
QState QHsmTst_idle(QHsmTst *me) {

    switch (Q_SIG(me)){
        case Q_ENTRY_SIG: {
            printf("idle\n");
            return Q_HANDLED();
        }

        case Q_EXIT_SIG: {
            printf("\t\t P: %d PE: %d\n",psim, pele);
            return Q_HANDLED();
        }

        case Q_OUT_FIRST: {
            if (curr == 1){
                psim++;
                pele++;
                return Q_HANDLED();
            }
            else{

                if (!inQueue(qout,1)){
                    pout++;
                    psim++;
                }
                enQueue(&qout, 1);
                EleDir = decideDir(curr, EleDir);
                return Q_TRAN(&QHsmTst_moving);
            }
        }

        case Q_OUT_SECOND: {
            if (curr == 2){
                psim++;
                pele++;
                return Q_HANDLED();
            }
            else{

                if (!inQueue(qout,2)){
                    pout++;
                    psim++;
                }
                enQueue(&qout, 2);
                EleDir = decideDir(curr, EleDir);
                return Q_TRAN(&QHsmTst_moving);
            }
        }

        case Q_OUT_THIRD: {
            if (curr == 3){
                psim++;
                pele++;
                return Q_HANDLED();
            }
            else{

                if (!inQueue(qout,3)){
                    pout++;
                    psim++;
                }
                enQueue(&qout, 3);
                EleDir = decideDir(curr, EleDir);
                return Q_TRAN(&QHsmTst_moving);
            }
        }

        case Q_OUT_FORTH: {

            if (curr == 4){
                psim++;
                pele++;
                return Q_HANDLED();
            }
            else{

                if (!inQueue(qout,4)){
                    pout++;
                    psim++;
                }
                enQueue(&qout, 4);
                EleDir = decideDir(curr, EleDir);
                return Q_TRAN(&QHsmTst_moving);
            }
        }

        case Q_OUT_FIFTH: {
            if (curr == 5){
                psim++;
                pele++;
                return Q_HANDLED();
            }
            else{

                if (!inQueue(qout,5)){
                    pout++;
                    psim++;
                }
                enQueue(&qout, 5);
                EleDir = decideDir(curr, EleDir);
                printf("curr %d dir %d", curr, EleDir);
                return Q_TRAN(&QHsmTst_moving);
            }
        }

        case Q_NOP:{

            return Q_HANDLED();

        }

        case TERMINATE_SIG: {
            BSP_exit();
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}

QState QHsmTst_moving(QHsmTst *me) {
    //printf("countSec %d\n", countSec);
    //printf("direction %d\n", EleDir);
    //printf("moving func\n");
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            //printf("moving\n");
            //printf("\t\tP: %d PE: %d\n",psim, pele);
            countSec = 0;
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            printf("exit moving");
            return Q_HANDLED();
        }
        case Q_OUT_FIRST: {
            //displayQueue(qout);
            //printf("inqueue %d\n", inQueue(qout,1));
            if (!inQueue(qout,1)){
                pout++;
                psim++;
            }
            enQueue(&qout, 1);
            countSec++;
            if(countSec == MOVE_TIME){
                countSec = 0;
                if(decideStop(curr, EleDir)){
                    return Q_TRAN(&QHsmTst_stop);
                }
            }
            return Q_HANDLED();
        }
        case Q_OUT_SECOND: {


            if (!inQueue(qout,2)){
                pout++;
                psim++;
            }
            enQueue(&qout, 2);
            countSec++;
            if(countSec == MOVE_TIME){
                countSec = 0;
                if(decideStop(curr, EleDir)){
                    return Q_TRAN(&QHsmTst_stop);
                }
            }
            return Q_HANDLED();
        }
        case Q_OUT_THIRD: {

            if (!inQueue(qout,3)){
                pout++;
                psim++;
            }
            enQueue(&qout, 3);
            countSec++;
            if(countSec == MOVE_TIME){
                countSec = 0;
                if(decideStop(curr, EleDir)){
                    return Q_TRAN(&QHsmTst_stop);
                }
            }
            return Q_HANDLED();
        }
        case Q_OUT_FORTH: {

            if (!inQueue(qout,4)){
                pout++;
                psim++;
            }
            enQueue(&qout, 4);
            countSec++;
            if(countSec == MOVE_TIME){
                countSec = 0;
                if(decideStop(curr, EleDir)){
                    return Q_TRAN(&QHsmTst_stop);
                }
            }

            return Q_HANDLED();
        }
        case Q_OUT_FIFTH: {

            if (!inQueue(qout,5)){
                pout++;
                psim++;
            }
            enQueue(&qout, 5);
            countSec++;
            if(countSec == MOVE_TIME){
                countSec = 0;
                if(decideStop(curr, EleDir)){
                    return Q_TRAN(&QHsmTst_stop);
                }
            }
            return Q_HANDLED();
        }

        case Q_NOP :{
            printf("\t\tP: %d PE: %d\n",psim, pele);

            countSec++;
            if(countSec == MOVE_TIME){
                countSec = 0;
                if(decideStop(curr, EleDir)){
                    return Q_TRAN(&QHsmTst_stop);
                }
            }
            return Q_HANDLED();

        }
        case TERMINATE_SIG: {
            BSP_exit();
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);

}


QState QHsmTst_stop(QHsmTst *me) {
    printf("countSec %d\n",countSec);
    switch(Q_SIG(me)){
        case Q_ENTRY_SIG: {
            if(inQueue(qin, curr)){
                remEleQueue(&qin, curr);
                pele--;
                psim--;

                printf("cp1\n");
            }

            if (inQueue(qout, curr)){
                remEleQueue(&qout, curr);
                pout--;
                pele++;
                if (m[curr-1].ptr != 0){
                m[curr-1].count++;
                m[curr-1].accumTime += simCount - m[curr-1].ptr;
                m[curr-1].avg = 1.0 * m[curr-1].accumTime/ m[curr-1].count;}
                int r = rand() % 5 + 1;

                enQueue(&qin, r);
                printf("cp2\n");
            }
            fprintf(l_outFile,"T: %lu P: %d PE: %d F: %d D: %s\n", simCount, lenQueue(qout)+lenQueue(qin), lenQueue(qin), curr, (EleDir == UP)? "UP" : "DOWN");
            printf("T: %lu P: %d PE: %d F: %d D: %s\n", simCount, psim, pele, curr, (EleDir == UP)? "UP" : "DOWN");


            //m[curr].ptr = 0;
            printf("m[%d] = {%lu, %lu, %lu, %f}\n", curr, m[curr-1].ptr, m[curr-1].count, m[curr-1].accumTime, m[curr-1].avg);

            printf("stop\n");
            countSec = 0;

            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            EleDir = decideDir(curr, EleDir);
            //stop = 0;
/*            if (!isEmptyQueue(qout)){
                EleDir = desideDir(qout, curr, EleDir);
                return Q_HANDLED();
            }
            else {
                if (!isEmptyQueue(qout)){
                    EleDir = (curr - headQueue(qout)) > 0 ? UP : DOWN;
                    return Q_HANDLED();
                }
            }*/
            return Q_HANDLED();
        }
        case Q_OUT_FIRST: {
            if (curr == 1){
                pele++;
                psim++;
                return Q_HANDLED();
            }
            else{
                if (!inQueue(qout,1)){
                    pout++;
                    psim++;
                }
                enQueue(&qout, 1);
                return Q_HANDLED();
            }
        }
        case Q_OUT_SECOND: {
            if (curr == 2){
                pele++;
                psim++;
                return Q_HANDLED();
            }
            else{
                if (!inQueue(qout,2)){
                    pout++;
                    psim++;
                }
                enQueue(&qout, 2);
                return Q_HANDLED();
            }
        }
        case Q_OUT_THIRD: {
            if (curr == 3){
                pele++;
                psim++;
                return Q_HANDLED();
            }
            else{
                if (!inQueue(qout,3)){
                    pout++;
                    psim++;
                }
                enQueue(&qout, 3);
                return Q_HANDLED();
            }
        }
        case Q_OUT_FORTH: {
            if (curr == 4){
                pele++;
                psim++;
                return Q_HANDLED();
            }
            else{
                if (!inQueue(qout,4)){
                    pout++;
                    psim++;
                }
                enQueue(&qout, 4);
                return Q_HANDLED();
            }
        }
        case Q_OUT_FIFTH: {
            if (curr == 5){
                pele++;
                psim++;
                return Q_HANDLED();
            }
            else{
                if (!inQueue(qout,4)){
                    pout++;
                    psim++;
                }
                enQueue(&qout, 5);
                return Q_HANDLED();
            }
        }

        case Q_NOP: {
            if(countSec == 10){
                if (!isEmptyQueue(qin) || !isEmptyQueue(qout)){
                    return Q_TRAN(&QHsmTst_moving);
                }
                else{
                    return Q_TRAN(&QHsmTst_idle);
                }
            }
            countSec++;
            return Q_HANDLED();

        }
        case TERMINATE_SIG: {
            BSP_exit();
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);

}
