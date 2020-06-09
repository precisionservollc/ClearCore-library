/*
 * AllTests.h
 *
 * Created: 9/11/2019 3:38:52 PM
 *  Author: scott_mayne
 */ 


#ifndef ALLTESTS_H_
#define ALLTESTS_H_

#include "testHooks.h"

IMPORT_TEST_GROUP(IO0Test);
IMPORT_TEST_GROUP(MtrTest);
IMPORT_TEST_GROUP(StepGenTest);
// BK Commented out due to failures \TODO Fix
//IMPORT_TEST_GROUP(AdcTest);
IMPORT_TEST_GROUP(TimingTest);
// Note: CCIO test requires at least one CCIO module connected to COM-1 
//IMPORT_TEST_GROUP(CcioTest);
IMPORT_TEST_GROUP(DITest);
IMPORT_TEST_GROUP(DIOTest);
IMPORT_TEST_GROUP(HBTest);
IMPORT_TEST_GROUP(SdCardTest);
// Note: When running Serial test, use SerialUsb for output.
// BK Commented out due to failures \TODO Fix
//IMPORT_TEST_GROUP(SerialTest);

#endif /* ALLTESTS_H_ */