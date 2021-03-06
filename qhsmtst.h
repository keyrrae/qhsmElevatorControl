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
#ifndef qhsmtst_h
#define qhsmtst_h

enum QHsmTstSignals {
    Q_NOP = Q_USER_SIG,
    Q_OUT_FIRST,
    Q_OUT_SECOND,
    Q_OUT_THIRD,
    Q_OUT_FORTH,
    Q_OUT_FIFTH,
    Q_IN_FIRST,
    Q_IN_SECOND,
    Q_IN_THIRD,
    Q_IN_FORTH,
    Q_IN_FIFTH,


    TERMINATE_SIG,
    IGNORE_SIG,
    MAX_SIG
};

extern struct QHsmTstTag HSM_QHsmTst;       /* the sole instance of QHsmTst */

void QHsmTst_ctor(void);              /* instantiate and initialize the HSM */

/* Board Support Package */
void BSP_display(char const *msg);
void BSP_exit(void);

#endif                                                         /* qhsmtst_h */
