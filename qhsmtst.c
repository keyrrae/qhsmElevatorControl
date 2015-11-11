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
#define UP 1
#define DOWN 2

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
static QState QHsmTst_approx   (QHsmTst *me);                  /* state-handler */
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
    simTime = 0;
    qin.rear = 0;
    qout.rear = 0;
    BSP_display("top-INIT;");
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
            return Q_HANDLED();
        }

        case Q_OUT_FIRST: {
            if (checkUpDown(&qout, 1)){
                return Q_TRAN(&QHsmTst_moving);
            }
            else
                return Q_HANDLED();
        }

        case Q_OUT_SECOND: {

            if (checkUpDown(&qout, 2)){
                return Q_TRAN(&QHsmTst_moving);
            }
            else
                return Q_HANDLED();
        }

        case Q_OUT_THIRD: {

            if (checkUpDown(&qout, 3)){
                //updateOut();
                return Q_TRAN(&QHsmTst_moving);
            }
            else
                return Q_HANDLED();
        }

        case Q_OUT_FORTH: {

            if (checkUpDown(&qout, 4)){
                //updateOut();
                return Q_TRAN(&QHsmTst_moving);
            }
            else
                return Q_HANDLED();
        }

        case Q_OUT_FIFTH: {

            if (checkUpDown(&qout, 5)){
                //updateOut();
                return Q_TRAN(&QHsmTst_moving);
            }
            else
                return Q_HANDLED();
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


    printf("moving func\n");
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            printf("moving\n");
            countSec = 0;
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            return Q_HANDLED();
        }
        case Q_OUT_FIRST: {
            countSec++;
            checkUpDown(&qout, 1);
            return Q_HANDLED();
        }
        case Q_OUT_SECOND: {

            countSec++;
            checkUpDown(&qout, 2);
            return Q_HANDLED();
        }
        case Q_OUT_THIRD: {

            countSec++;
            checkUpDown(&qout, 3);
            return Q_HANDLED();
        }
        case Q_OUT_FORTH: {

            countSec++;
            checkUpDown(&qout, 4);
            return Q_HANDLED();
        }
        case Q_OUT_FIFTH: {

            countSec++;
            checkUpDown(&qout, 5);
            return Q_HANDLED();
        }

        case Q_IN_FIRST: {

            countSec++;
            enQueue(&qin, 1);
            return Q_HANDLED();
        }

        case Q_IN_SECOND: {

            countSec++;
            enQueue(&qin, 2);
            return Q_HANDLED();
        }
        case Q_IN_THIRD: {

            countSec++;
            enQueue(&qin, 3);
            return Q_HANDLED();
        }
        case Q_IN_FORTH: {

            countSec++;
            enQueue(&qin, 4);
            return Q_HANDLED();
        }
        case Q_IN_FIFTH: {

            countSec++;
            enQueue(&qin, 5);
            return Q_HANDLED();
        }
        case Q_NOP :{

            countSec++;
            if(countSec == 5){
                countSec = 0;
                if (curr == 1 && EleDir == DOWN) {
                        EleDir = UP;
                }

                else if (curr == 5 && EleDir == UP){
                    EleDir = DOWN;
                }
                else {
                    curr += (EleDir == UP) ? 1 : -1;
                }
                printf("%d\n",curr);
                if(inQueue(qin, curr)){
                    remEleQueue(&qin, curr);
                    pele--;
                    psim--;

                    printf("cp1\n");
                    return Q_TRAN(&QHsmTst_stop);
                }
                else if (inQueue(qout, curr)){
                    remEleQueue(&qout, curr);
                    pout--;
                    pele++;
                    printf("cp2\n");
                    return Q_TRAN(&QHsmTst_stop);
                }
                else{
                    printf("cp3\n");
                    return Q_HANDLED();//Q_TRAN(&QHsmTst_moving);//moving);
                }
                //return Q_TRAN(&QHsmTst_approx);
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

QState QHsmTst_approx(QHsmTst *me) {
    switch(Q_SIG(me)) {
        case Q_ENTRY_SIG:{
            printf("approx\n");
            if (curr == 1 && EleDir == DOWN) {
                    EleDir = UP;
            }

            else if (curr == 5 && EleDir == UP){
                EleDir = DOWN;
            }
            else {
                curr += (EleDir == UP) ? 1 : -1;
            }
printf("%d\n",curr);
            if(inQueue(qin, curr)){
                remEleQueue(&qin, curr);
                pele--;
                psim--;

                printf("cp1\n");
                return Q_TRAN(&QHsmTst_stop);
            }
            else if (inQueue(qout, curr)){
                remEleQueue(&qout, curr);
                pout--;
                pele++;
                printf("cp2\n");
                return Q_TRAN(&QHsmTst_stop);
            }
            else{
                printf("cp3\n");
                return Q_TRAN(&QHsmTst_idle);//moving);
            }
            printf("cp4\n");
            return Q_HANDLED();
        }
printf("cp\n");

        case Q_EXIT_SIG:{
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
    switch(Q_SIG(me)){
        case Q_ENTRY_SIG: {
            printf("stop\n");
            countSec = 0;
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            if (!isEmptyQueue(qin)){
                EleDir = (curr - headQueue(qin)) > 0 ? UP : DOWN;
                return Q_HANDLED();
            }
            else {
                if (!isEmptyQueue(qout)){
                    EleDir = (curr - headQueue(qout)) > 0 ? UP : DOWN;
                    return Q_HANDLED();
                }
            }
            return Q_HANDLED();
        }
        case Q_OUT_FIRST: {

            enQueue(&qout, 1);
            return Q_HANDLED();
        }
        case Q_OUT_SECOND: {

            enQueue(&qout, 2);
            return Q_HANDLED();
        }
        case Q_OUT_THIRD: {

            enQueue(&qout, 3);
            return Q_HANDLED();
        }
        case Q_OUT_FORTH: {

            enQueue(&qout, 4);
            return Q_HANDLED();
        }
        case Q_OUT_FIFTH: {

            enQueue(&qout, 5);
            return Q_HANDLED();
        }

        case Q_IN_FIRST: {
            enQueue(&qin, 1);
            return Q_HANDLED();
        }

        case Q_IN_SECOND: {
            enQueue(&qin, 2);
            return Q_HANDLED();
        }
        case Q_IN_THIRD: {
            enQueue(&qin, 3);
            return Q_HANDLED();
        }
        case Q_IN_FORTH: {
            enQueue(&qin, 4);
            return Q_HANDLED();
        }
        case Q_IN_FIFTH: {
            enQueue(&qin, 5);
            //updateIn();
            return Q_HANDLED();
        }
        case Q_NOP :{
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

/*
QState QHsmTst_s211(QHsmTst *me) {
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            BSP_display("s211-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            BSP_display("s211-EXIT;");
            return Q_HANDLED();
        }
        case D_SIG: {
            BSP_display("s211-D;");
            return Q_TRAN(&QHsmTst_s21);
        }
        case F_SIG: {
            BSP_display("s211-F;");
            return Q_TRAN(&QHsmTst_s1);
        }
        case G_SIG: {
            BSP_display("s211-G;");
            return Q_TRAN(&QHsmTst_s111);
        }
    }
    return Q_SUPER(&QHsmTst_s21);
}
*/
