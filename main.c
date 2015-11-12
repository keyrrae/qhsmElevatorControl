/*****************************************************************************
* QHsmTst example
* Last Updated for Version: 4.0.00
* Date of the Last Update:  Apr 05, 2008
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
#include <stdlib.h>
#include <stdio.h>
#include "elev.h"

/* local objects -----------------------------------------------------------*/
FILE *l_outFile = (FILE *)0;
static unsigned int freq = 0;
static unsigned long simTime = 0;
unsigned long simCount=0;
static char stop = 0;
static void dispatch(QSignal sig);

struct measure m[5] = {{0, 0, 0, 0.0},
                       {0, 0, 0, 0.0},
                       {0, 0, 0, 0.0},
                       {0, 0, 0, 0.0},
                       {0, 0, 0, 0.0}};

/*..........................................................................*/
int main(int argc, char *argv[]) {
    QHsmTst_ctor();                                  /* instantiate the HSM */

    if (argc >= 3) {                                  /* file name provided? */
        l_outFile = fopen("results.out", "w");
        freq = atoi(argv[1]);
        simTime = atol(argv[2]);
    }

    if (l_outFile == (FILE *)0) {                   /* interactive version? */
        l_outFile = stdout;            /* use the stdout as the output file */

        printf("QHsmTst example, built on %s at %s,\n"
               "QP-nano: %s.\nPress ESC to quit...\n",
               __DATE__, __TIME__, QP_getVersion());

        QHsm_init((QHsm *)&HSM_QHsmTst);    /* take the initial transitioin */
/*
        for (;;) {                                            // event loop
            int c;

            printf("\n>");
            c = getchar();
            printf(": ");

            if ('a' <= c && c <= 'i') {                        // in range?
                Q_SIG((QHsm *)&HSM_QHsmTst) = (QSignal)(c - 'a' + A_SIG);
            }
            else if ('A' <= c && c <= 'I') {                   // in range?
                Q_SIG((QHsm *)&HSM_QHsmTst) = (QSignal)(c - 'A' + A_SIG);
            }
            else if (c == '\33') {                          // the ESC key?
                Q_SIG((QHsm *)&HSM_QHsmTst) = TERMINATE_SIG;
            }
            else {
                Q_SIG((QHsm *)&HSM_QHsmTst) = IGNORE_SIG;
            }

            QHsm_dispatch((QHsm *)&HSM_QHsmTst);      // dispatch the event
        }
*/
    }
    else {                                                 /* batch version */
        printf("QHsmTst example, built on %s at %s, QP-nano %s\n"
               "output saved to %s\n",
               __DATE__, __TIME__, QP_getVersion(),
               argv[1]);

        //fprintf(l_outFile, "QHsmTst example, QP-nano %s\n",
        //        QP_getVersion());

        QHsm_init((QHsm *)&HSM_QHsmTst);    /* take the initial transitioin */

                                     /* testing all kinds of transitions... */


 	    unsigned long i;
        int j;
        int r;
        if (argc == 4){
            srand(atoi(argv[3]));
        }
        else {
            srand(234);
        }

        for(i = 0; i < simTime / freq; ++i){
            r = rand() % 5;
            printf("========%d=========================\n",r+1);

            switch (r) {
                case 0:
                    dispatch(Q_OUT_FIRST); break;
                case 1:
                    dispatch(Q_OUT_SECOND); break;
                case 2:
                    dispatch(Q_OUT_THIRD); break;
                case 3:
                    dispatch(Q_OUT_FORTH); break;
                case 4:
                    dispatch(Q_OUT_FIFTH); break;
                default:
                    break;
            }
            m[r].ptr = simCount;
            printf("m[%d].ptr = %lu\n",r,m[r].ptr);
            printf("simtime: %lu\n", simCount);
            simCount++;

            for (j = 0; j < freq-1; j++){
                dispatch(Q_NOP);
                printf("=========NOP==========\n");
                printf("simtime: %lu\n", simCount);
                simCount++;

            }
        }


/*
        dispatch(Q_OUT_FIFTH);
        for (int i = 0;i <128;i++){

            printf("i = %d\n",i);
            dispatch(Q_NOP);
        }
*/

for (i = 0; i<5; i++){
    fprintf(l_outFile, "%lu: %f  ",i+1, m[i].avg);
}
fprintf(l_outFile, "\n");
        fclose(l_outFile);
    }

    return 0;
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    exit(-1);
}
/*..........................................................................*/
void BSP_display(char const *msg) {
    fprintf(l_outFile, msg);
}
/*..........................................................................*/
void BSP_exit(void) {
    printf("Bye, Bye!");
    exit(0);
}
/*..........................................................................*/
static void dispatch(QSignal sig) {
    //fprintf(l_outFile, "\n%c:", 'A' + sig - A_SIG);
    //printf("dispatch\n");
    Q_SIG((QHsm *)&HSM_QHsmTst) = sig;
    QHsm_dispatch((QHsm *)&HSM_QHsmTst);              /* dispatch the event */
}
