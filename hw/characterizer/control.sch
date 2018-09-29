EESchema Schematic File Version 4
LIBS:characterizer-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 8
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 7850 2000 2    50   Output ~ 0
ICHG_A
Text HLabel 7050 1350 2    50   Output ~ 0
IDCHG_B
Text HLabel 7000 3050 2    50   Input ~ 0
CC_A
Text HLabel 7000 3100 2    50   Input ~ 0
CC_B
Text HLabel 7000 3200 2    50   Input ~ 0
DC_A
Text HLabel 7000 3250 2    50   Input ~ 0
DC_B
Text HLabel 6550 3550 2    50   Input ~ 0
VBAT0
Text HLabel 7300 3400 2    50   Input ~ 0
VBAT1
Text HLabel 7050 2000 2    50   Output ~ 0
ICHG_B
Text HLabel 7850 1350 2    50   Output ~ 0
IDCHG_A
$Comp
L Device:R_Small R21
U 1 1 5B9F7EA5
P 7150 3400
F 0 "R21" V 7100 3200 50  0000 C CNN
F 1 "10k" V 7050 3550 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 7150 3400 50  0001 C CNN
F 3 "~" H 7150 3400 50  0001 C CNN
	1    7150 3400
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R19
U 1 1 5B9F7F15
P 7050 3550
F 0 "R19" H 7109 3596 50  0000 L CNN
F 1 "10k" H 7109 3505 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 7050 3550 50  0001 C CNN
F 3 "~" H 7050 3550 50  0001 C CNN
	1    7050 3550
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R13
U 1 1 5B9F7FC9
P 6300 3700
F 0 "R13" H 6359 3746 50  0000 L CNN
F 1 "10k" H 6359 3655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 6300 3700 50  0001 C CNN
F 3 "~" H 6300 3700 50  0001 C CNN
	1    6300 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R15
U 1 1 5B9F7FFB
P 6400 3550
F 0 "R15" V 6300 3700 50  0000 C CNN
F 1 "10k" V 6295 3550 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 6400 3550 50  0001 C CNN
F 3 "~" H 6400 3550 50  0001 C CNN
	1    6400 3550
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0112
U 1 1 5B9F81B4
P 7050 3850
F 0 "#PWR0112" H 7050 3600 50  0001 C CNN
F 1 "GND" H 7055 3677 50  0000 C CNN
F 2 "" H 7050 3850 50  0001 C CNN
F 3 "" H 7050 3850 50  0001 C CNN
	1    7050 3850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0113
U 1 1 5B9F81E5
P 6300 3850
F 0 "#PWR0113" H 6300 3600 50  0001 C CNN
F 1 "GND" H 6305 3677 50  0000 C CNN
F 2 "" H 6300 3850 50  0001 C CNN
F 3 "" H 6300 3850 50  0001 C CNN
	1    6300 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 3850 7050 3650
Wire Wire Line
	7050 3450 7050 3400
Wire Wire Line
	6300 3600 6300 3550
Connection ~ 6300 3550
Wire Wire Line
	6300 3550 6300 3450
Wire Wire Line
	6300 3850 6300 3800
Wire Wire Line
	6550 3550 6500 3550
Wire Wire Line
	7300 3400 7250 3400
$Comp
L Device:R_Small R24
U 1 1 5B9F9A58
P 7600 2000
F 0 "R24" V 7650 1850 50  0000 C CNN
F 1 "4k02" V 7750 1900 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 7600 2000 50  0001 C CNN
F 3 "~" H 7600 2000 50  0001 C CNN
	1    7600 2000
	0    1    1    0   
$EndComp
$Comp
L Device:C_Small C14
U 1 1 5B9F9A5F
P 7750 2100
F 0 "C14" H 7842 2146 50  0000 L CNN
F 1 "100n" H 7842 2055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7750 2100 50  0001 C CNN
F 3 "~" H 7750 2100 50  0001 C CNN
	1    7750 2100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0114
U 1 1 5B9F9A66
P 7750 2250
F 0 "#PWR0114" H 7750 2000 50  0001 C CNN
F 1 "GND" H 7755 2077 50  0000 C CNN
F 2 "" H 7750 2250 50  0001 C CNN
F 3 "" H 7750 2250 50  0001 C CNN
	1    7750 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 2250 7750 2200
Wire Wire Line
	7750 2000 7700 2000
$Comp
L Device:R_Small R17
U 1 1 5B9F9D7A
P 6800 1350
F 0 "R17" V 6850 1150 50  0000 C CNN
F 1 "4k02" V 6950 1100 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 6800 1350 50  0001 C CNN
F 3 "~" H 6800 1350 50  0001 C CNN
	1    6800 1350
	0    1    1    0   
$EndComp
$Comp
L Device:C_Small C11
U 1 1 5B9F9D81
P 6950 1450
F 0 "C11" H 7042 1496 50  0000 L CNN
F 1 "100n" H 7042 1405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6950 1450 50  0001 C CNN
F 3 "~" H 6950 1450 50  0001 C CNN
	1    6950 1450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0115
U 1 1 5B9F9D88
P 6950 1600
F 0 "#PWR0115" H 6950 1350 50  0001 C CNN
F 1 "GND" H 6955 1427 50  0000 C CNN
F 2 "" H 6950 1600 50  0001 C CNN
F 3 "" H 6950 1600 50  0001 C CNN
	1    6950 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 1600 6950 1550
Wire Wire Line
	6950 1350 6900 1350
$Comp
L Device:R_Small R23
U 1 1 5B9FA10B
P 7600 1350
F 0 "R23" V 7650 1200 50  0000 C CNN
F 1 "4k02" V 7750 1250 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 7600 1350 50  0001 C CNN
F 3 "~" H 7600 1350 50  0001 C CNN
	1    7600 1350
	0    1    1    0   
$EndComp
$Comp
L Device:C_Small C13
U 1 1 5B9FA112
P 7750 1450
F 0 "C13" H 7842 1496 50  0000 L CNN
F 1 "100n" H 7842 1405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7750 1450 50  0001 C CNN
F 3 "~" H 7750 1450 50  0001 C CNN
	1    7750 1450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0116
U 1 1 5B9FA119
P 7750 1600
F 0 "#PWR0116" H 7750 1350 50  0001 C CNN
F 1 "GND" H 7755 1427 50  0000 C CNN
F 2 "" H 7750 1600 50  0001 C CNN
F 3 "" H 7750 1600 50  0001 C CNN
	1    7750 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 1600 7750 1550
Wire Wire Line
	7750 1350 7700 1350
$Comp
L Device:R_Small R18
U 1 1 5B9FA525
P 6800 2000
F 0 "R18" V 6850 1800 50  0000 C CNN
F 1 "4k02" V 6950 1850 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 6800 2000 50  0001 C CNN
F 3 "~" H 6800 2000 50  0001 C CNN
	1    6800 2000
	0    1    1    0   
$EndComp
$Comp
L Device:C_Small C12
U 1 1 5B9FA52C
P 6950 2100
F 0 "C12" H 7042 2146 50  0000 L CNN
F 1 "100n" H 7042 2055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6950 2100 50  0001 C CNN
F 3 "~" H 6950 2100 50  0001 C CNN
	1    6950 2100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0117
U 1 1 5B9FA533
P 6950 2250
F 0 "#PWR0117" H 6950 2000 50  0001 C CNN
F 1 "GND" H 6955 2077 50  0000 C CNN
F 2 "" H 6950 2250 50  0001 C CNN
F 3 "" H 6950 2250 50  0001 C CNN
	1    6950 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 2250 6950 2200
Wire Wire Line
	6950 2000 6900 2000
Wire Wire Line
	5700 1250 7500 1250
Wire Wire Line
	7500 1250 7500 1350
Wire Wire Line
	5750 1350 6700 1350
Wire Wire Line
	6950 1350 7050 1350
Connection ~ 6950 1350
Wire Wire Line
	7750 1350 7850 1350
Connection ~ 7750 1350
Wire Wire Line
	7400 2000 7500 2000
Wire Wire Line
	5700 1250 5700 2800
Wire Wire Line
	5700 2800 5450 2800
Wire Wire Line
	5450 2850 5750 2850
Wire Wire Line
	5750 2850 5750 1350
Wire Wire Line
	7400 1900 7400 2000
Wire Wire Line
	7850 2000 7750 2000
Connection ~ 7750 2000
Wire Wire Line
	7050 2000 6950 2000
Connection ~ 6950 2000
Text HLabel 5050 2000 0    50   Output ~ 0
SCL
Text HLabel 5050 1950 0    50   BiDi ~ 0
SDA
$Comp
L Device:R_Small R11
U 1 1 5BA0D22D
P 5200 1750
F 0 "R11" V 5250 1750 50  0000 C CNN
F 1 "3k3" V 5350 1750 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 5200 1750 50  0001 C CNN
F 3 "~" H 5200 1750 50  0001 C CNN
	1    5200 1750
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small R12
U 1 1 5BA0D2BB
P 5300 1750
F 0 "R12" V 5200 1750 50  0000 C CNN
F 1 "3k3" V 5100 1750 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 5300 1750 50  0001 C CNN
F 3 "~" H 5300 1750 50  0001 C CNN
	1    5300 1750
	-1   0    0    1   
$EndComp
Wire Wire Line
	5450 2600 5500 2600
Wire Wire Line
	5500 2600 5500 2000
Wire Wire Line
	5500 2000 5300 2000
Wire Wire Line
	5050 1950 5200 1950
Wire Wire Line
	5550 1950 5550 2650
Wire Wire Line
	5550 2650 5450 2650
Wire Wire Line
	5200 1850 5200 1950
Connection ~ 5200 1950
Wire Wire Line
	5200 1950 5550 1950
Wire Wire Line
	5300 1850 5300 2000
Connection ~ 5300 2000
Wire Wire Line
	5300 2000 5050 2000
Wire Wire Line
	5200 1650 5200 1550
Wire Wire Line
	5200 1550 5300 1550
Wire Wire Line
	5300 1550 5300 1650
$Comp
L Device:C_Small C5
U 1 1 5BA148C8
P 3550 2700
F 0 "C5" H 3600 2800 50  0000 L CNN
F 1 "100n" H 3600 2650 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3550 2700 50  0001 C CNN
F 3 "~" H 3550 2700 50  0001 C CNN
	1    3550 2700
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C7
U 1 1 5BA1671E
P 3800 2700
F 0 "C7" H 3850 2800 50  0000 L CNN
F 1 "100n" H 3850 2650 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3800 2700 50  0001 C CNN
F 3 "~" H 3800 2700 50  0001 C CNN
	1    3800 2700
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C9
U 1 1 5BA16756
P 4050 2700
F 0 "C9" H 4100 2800 50  0000 L CNN
F 1 "100n" H 4100 2650 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4050 2700 50  0001 C CNN
F 3 "~" H 4050 2700 50  0001 C CNN
	1    4050 2700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0118
U 1 1 5BA167B9
P 3350 2800
F 0 "#PWR0118" H 3350 2550 50  0001 C CNN
F 1 "GND" H 3355 2627 50  0000 C CNN
F 2 "" H 3350 2800 50  0001 C CNN
F 3 "" H 3350 2800 50  0001 C CNN
	1    3350 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 2800 4400 2800
Connection ~ 3800 2800
Wire Wire Line
	3800 2800 3550 2800
Connection ~ 4050 2800
Wire Wire Line
	4050 2800 3800 2800
Wire Wire Line
	4450 2700 4400 2700
Wire Wire Line
	4400 2700 4400 2650
Wire Wire Line
	4400 2600 4450 2600
Wire Wire Line
	4400 2650 4450 2650
Connection ~ 4400 2650
Wire Wire Line
	4400 2650 4400 2600
Wire Wire Line
	3550 2550 3800 2550
Wire Wire Line
	4400 2550 4400 2600
Wire Wire Line
	3550 2550 3550 2600
Connection ~ 4400 2600
Wire Wire Line
	4050 2600 4050 2550
Connection ~ 4050 2550
Wire Wire Line
	4050 2550 4400 2550
Wire Wire Line
	3800 2550 3800 2600
Connection ~ 3800 2550
Wire Wire Line
	3800 2550 4050 2550
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J2
U 1 1 5BA3A224
P 2350 2850
F 0 "J2" H 2400 3267 50  0000 C CNN
F 1 "Conn_02x05_Odd_Even" H 2400 3176 50  0000 C CNN
F 2 "project:CONN_JTAG_10" H 2350 2850 50  0001 C CNN
F 3 "~" H 2350 2850 50  0001 C CNN
F 4 "1175-1629-ND" H 0   0   50  0001 C CNN "Digikey PN"
	1    2350 2850
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R7
U 1 1 5BA3A2FB
P 2900 3550
F 0 "R7" H 2959 3596 50  0000 L CNN
F 1 "DNI" H 2959 3505 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 2900 3550 50  0001 C CNN
F 3 "~" H 2900 3550 50  0001 C CNN
	1    2900 3550
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R8
U 1 1 5BA3A35B
P 2900 3800
F 0 "R8" H 2959 3846 50  0000 L CNN
F 1 "10k" H 2959 3755 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 2900 3800 50  0001 C CNN
F 3 "~" H 2900 3800 50  0001 C CNN
	1    2900 3800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0119
U 1 1 5BA3A3C8
P 2900 4000
F 0 "#PWR0119" H 2900 3750 50  0001 C CNN
F 1 "GND" H 2905 3827 50  0000 C CNN
F 2 "" H 2900 4000 50  0001 C CNN
F 3 "" H 2900 4000 50  0001 C CNN
	1    2900 4000
	1    0    0    -1  
$EndComp
$Comp
L Device:Crystal Y1
U 1 1 5BA3A4C3
P 3800 3450
F 0 "Y1" H 3750 3300 50  0000 C CNN
F 1 "ABLS-8.000MHZ-B4-T" H 3800 3200 50  0001 C CNN
F 2 "Crystal:Crystal_SMD_HC49-SD" H 3800 3450 50  0001 C CNN
F 3 "~" H 3800 3450 50  0001 C CNN
F 4 "535-10212-1-ND" H 3800 3450 50  0001 C CNN "Digikey PN"
	1    3800 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C8
U 1 1 5BA3A529
P 4000 3750
F 0 "C8" H 4050 3850 50  0000 L CNN
F 1 "18p" H 4050 3700 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4000 3750 50  0001 C CNN
F 3 "~" H 4000 3750 50  0001 C CNN
	1    4000 3750
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C6
U 1 1 5BA3A5BD
P 3600 3750
F 0 "C6" H 3650 3850 50  0000 L CNN
F 1 "18p" H 3650 3700 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3600 3750 50  0001 C CNN
F 3 "~" H 3600 3750 50  0001 C CNN
	1    3600 3750
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR0120
U 1 1 5BA3A775
P 3600 4000
F 0 "#PWR0120" H 3600 3750 50  0001 C CNN
F 1 "GND" H 3605 3827 50  0000 C CNN
F 2 "" H 3600 4000 50  0001 C CNN
F 3 "" H 3600 4000 50  0001 C CNN
	1    3600 4000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0121
U 1 1 5BA3A7B0
P 4000 4000
F 0 "#PWR0121" H 4000 3750 50  0001 C CNN
F 1 "GND" H 4005 3827 50  0000 C CNN
F 2 "" H 4000 4000 50  0001 C CNN
F 3 "" H 4000 4000 50  0001 C CNN
	1    4000 4000
	1    0    0    -1  
$EndComp
$Comp
L parts:USB_OTG J3
U 1 1 5BA3A891
P 2750 4900
F 0 "J3" H 2805 5367 50  0000 C CNN
F 1 "10118194-0001LF" H 2805 5276 50  0001 C CNN
F 2 "project:USB-microB-AMPH" H 2900 4850 50  0001 C CNN
F 3 " ~" H 2900 4850 50  0001 C CNN
F 4 "609-4618-1-ND" H 2750 4900 50  0001 C CNN "Digikey PN"
	1    2750 4900
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R10
U 1 1 5BA3A94A
P 3300 4600
F 0 "R10" V 3104 4600 50  0000 C CNN
F 1 "0" V 3195 4600 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 3300 4600 50  0001 C CNN
F 3 "~" H 3300 4600 50  0001 C CNN
	1    3300 4600
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R9
U 1 1 5BA3A9F8
P 3200 5100
F 0 "R9" H 3141 5054 50  0000 R CNN
F 1 "100k" H 3141 5145 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 3200 5100 50  0001 C CNN
F 3 "~" H 3200 5100 50  0001 C CNN
	1    3200 5100
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small R6
U 1 1 5BA3AA82
P 2700 5500
F 0 "R6" H 2600 5450 50  0000 R CNN
F 1 "1M" H 2600 5550 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 2700 5500 50  0001 C CNN
F 3 "~" H 2700 5500 50  0001 C CNN
	1    2700 5500
	-1   0    0    1   
$EndComp
$Comp
L Device:C_Small C4
U 1 1 5BA3AADE
P 2550 5500
F 0 "C4" H 2600 5600 50  0000 L CNN
F 1 "4n7" H 2600 5450 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2550 5500 50  0001 C CNN
F 3 "~" H 2550 5500 50  0001 C CNN
	1    2550 5500
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR0122
U 1 1 5BA3AB70
P 2550 5650
F 0 "#PWR0122" H 2550 5400 50  0001 C CNN
F 1 "GND" H 2555 5477 50  0000 C CNN
F 2 "" H 2550 5650 50  0001 C CNN
F 3 "" H 2550 5650 50  0001 C CNN
	1    2550 5650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0123
U 1 1 5BA3ABB5
P 3050 5550
F 0 "#PWR0123" H 3050 5300 50  0001 C CNN
F 1 "GND" H 3055 5377 50  0000 C CNN
F 2 "" H 3050 5550 50  0001 C CNN
F 3 "" H 3050 5550 50  0001 C CNN
	1    3050 5550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0124
U 1 1 5BA3ABFA
P 3200 5550
F 0 "#PWR0124" H 3200 5300 50  0001 C CNN
F 1 "GND" H 3205 5377 50  0000 C CNN
F 2 "" H 3200 5550 50  0001 C CNN
F 3 "" H 3200 5550 50  0001 C CNN
	1    3200 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 4600 3050 4600
Wire Wire Line
	3200 5200 3200 5550
Wire Wire Line
	2700 5300 2700 5350
Wire Wire Line
	2700 5600 2700 5650
Wire Wire Line
	2700 5650 2550 5650
Wire Wire Line
	2550 5650 2550 5600
Connection ~ 2550 5650
Wire Wire Line
	2550 5400 2550 5350
Connection ~ 2700 5350
Wire Wire Line
	2700 5350 2700 5400
Wire Wire Line
	3600 3650 3600 3450
Wire Wire Line
	3600 3450 3650 3450
Wire Wire Line
	3950 3450 4000 3450
Wire Wire Line
	4000 3450 4000 3650
Wire Wire Line
	4000 3450 4000 3400
Wire Wire Line
	4000 3400 4450 3400
Connection ~ 4000 3450
Wire Wire Line
	3600 3450 3600 3300
Wire Wire Line
	4200 3300 4200 3350
Wire Wire Line
	4200 3350 4450 3350
Wire Wire Line
	3600 3300 4200 3300
Connection ~ 3600 3450
Wire Wire Line
	4450 3250 3150 3250
Wire Wire Line
	2900 3700 2900 3650
Wire Wire Line
	2900 3900 2900 4000
Wire Wire Line
	2900 3400 2900 3450
Wire Wire Line
	3050 4800 4350 4800
Wire Wire Line
	4350 4800 4350 3600
Wire Wire Line
	4350 3600 4450 3600
Wire Wire Line
	3050 4900 4300 4900
Wire Wire Line
	4300 4900 4300 3550
Wire Wire Line
	4300 3550 4450 3550
$Comp
L power:+3.3V #PWR0125
U 1 1 5BA9B7C4
P 2900 3400
F 0 "#PWR0125" H 2900 3250 50  0001 C CNN
F 1 "+3.3V" H 2915 3573 50  0000 C CNN
F 2 "" H 2900 3400 50  0001 C CNN
F 3 "" H 2900 3400 50  0001 C CNN
	1    2900 3400
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0126
U 1 1 5BA9B881
P 3350 2550
F 0 "#PWR0126" H 3350 2400 50  0001 C CNN
F 1 "+3.3V" H 3365 2723 50  0000 C CNN
F 2 "" H 3350 2550 50  0001 C CNN
F 3 "" H 3350 2550 50  0001 C CNN
	1    3350 2550
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0127
U 1 1 5BA9B8C6
P 1950 2500
F 0 "#PWR0127" H 1950 2350 50  0001 C CNN
F 1 "+3.3V" H 1965 2673 50  0000 C CNN
F 2 "" H 1950 2500 50  0001 C CNN
F 3 "" H 1950 2500 50  0001 C CNN
	1    1950 2500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0128
U 1 1 5BA9B90B
P 1950 3150
F 0 "#PWR0128" H 1950 2900 50  0001 C CNN
F 1 "GND" H 1955 2977 50  0000 C CNN
F 2 "" H 1950 3150 50  0001 C CNN
F 3 "" H 1950 3150 50  0001 C CNN
	1    1950 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 2750 1950 2750
Wire Wire Line
	1950 2750 1950 2850
Wire Wire Line
	1950 2650 1950 2500
Wire Wire Line
	1950 2650 2150 2650
Wire Wire Line
	2150 2850 1950 2850
Connection ~ 1950 2850
Wire Wire Line
	1950 2850 1950 3050
Wire Wire Line
	1950 3050 2150 3050
Connection ~ 1950 3050
Wire Wire Line
	1950 3050 1950 3150
Wire Wire Line
	4450 3100 2850 3100
Wire Wire Line
	2850 3100 2850 2650
Wire Wire Line
	2850 2650 2650 2650
Wire Wire Line
	2650 2750 2800 2750
Wire Wire Line
	2800 3150 4450 3150
NoConn ~ 2650 2850
NoConn ~ 2650 2950
NoConn ~ 2150 2950
Wire Wire Line
	4450 2850 4400 2850
Wire Wire Line
	4400 2850 4400 2800
Connection ~ 4400 2800
Wire Wire Line
	4400 2800 4050 2800
Wire Wire Line
	2800 2750 2800 3150
Wire Wire Line
	3550 2800 3350 2800
Connection ~ 3550 2800
Wire Wire Line
	3350 2550 3550 2550
Connection ~ 3550 2550
Wire Wire Line
	2650 3050 3750 3050
$Comp
L power:+5V #PWR0129
U 1 1 5BB1DA37
P 3550 4600
F 0 "#PWR0129" H 3550 4450 50  0001 C CNN
F 1 "+5V" H 3565 4773 50  0000 C CNN
F 2 "" H 3550 4600 50  0001 C CNN
F 3 "" H 3550 4600 50  0001 C CNN
	1    3550 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 4600 3550 4600
$Comp
L power:+3.3V #PWR0130
U 1 1 5BBE0079
P 5200 1550
F 0 "#PWR0130" H 5200 1400 50  0001 C CNN
F 1 "+3.3V" H 5215 1723 50  0000 C CNN
F 2 "" H 5200 1550 50  0001 C CNN
F 3 "" H 5200 1550 50  0001 C CNN
	1    5200 1550
	1    0    0    -1  
$EndComp
Connection ~ 5200 1550
Text HLabel 6550 4400 2    50   Input ~ 0
VBAT2
Text HLabel 7300 4250 2    50   Input ~ 0
VBAT3
$Comp
L Device:R_Small R22
U 1 1 5BC6CA6A
P 7150 4250
F 0 "R22" V 7050 4400 50  0000 C CNN
F 1 "10k" V 7045 4250 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 7150 4250 50  0001 C CNN
F 3 "~" H 7150 4250 50  0001 C CNN
	1    7150 4250
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R20
U 1 1 5BC6CA71
P 7050 4400
F 0 "R20" H 7109 4446 50  0000 L CNN
F 1 "10k" H 7109 4355 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 7050 4400 50  0001 C CNN
F 3 "~" H 7050 4400 50  0001 C CNN
	1    7050 4400
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R14
U 1 1 5BC6CA78
P 6300 4550
F 0 "R14" H 6359 4596 50  0000 L CNN
F 1 "10k" H 6359 4505 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 6300 4550 50  0001 C CNN
F 3 "~" H 6300 4550 50  0001 C CNN
	1    6300 4550
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R16
U 1 1 5BC6CA7F
P 6400 4400
F 0 "R16" V 6300 4550 50  0000 C CNN
F 1 "10k" V 6295 4400 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 6400 4400 50  0001 C CNN
F 3 "~" H 6400 4400 50  0001 C CNN
	1    6400 4400
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0131
U 1 1 5BC6CA86
P 7050 4700
F 0 "#PWR0131" H 7050 4450 50  0001 C CNN
F 1 "GND" H 7055 4527 50  0000 C CNN
F 2 "" H 7050 4700 50  0001 C CNN
F 3 "" H 7050 4700 50  0001 C CNN
	1    7050 4700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0132
U 1 1 5BC6CA8C
P 6300 4700
F 0 "#PWR0132" H 6300 4450 50  0001 C CNN
F 1 "GND" H 6305 4527 50  0000 C CNN
F 2 "" H 6300 4700 50  0001 C CNN
F 3 "" H 6300 4700 50  0001 C CNN
	1    6300 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 4700 7050 4500
Wire Wire Line
	7050 4300 7050 4250
Wire Wire Line
	6300 4450 6300 4400
Connection ~ 6300 4400
Wire Wire Line
	6300 4400 6300 4300
Wire Wire Line
	6300 4700 6300 4650
Wire Wire Line
	6550 4400 6500 4400
Wire Wire Line
	7300 4250 7250 4250
Wire Wire Line
	7050 3400 5450 3400
Connection ~ 7050 3400
Wire Wire Line
	5450 3450 6300 3450
Wire Wire Line
	7050 4250 6150 4250
Wire Wire Line
	6150 4250 6150 3500
Wire Wire Line
	6150 3500 5450 3500
Connection ~ 7050 4250
Wire Wire Line
	5450 3550 6100 3550
Wire Wire Line
	6100 3550 6100 4300
Wire Wire Line
	6100 4300 6300 4300
$Comp
L local:STM32F042K6T6 U2
U 1 1 5B9F5096
P 4950 3150
F 0 "U2" H 4950 3947 60  0000 C CNN
F 1 "STM32F042K6T6" H 4950 3841 60  0000 C CNN
F 2 "Package_QFP:LQFP-32_7x7mm_P0.8mm" H 4350 2750 60  0001 C CNN
F 3 "" H 4350 2750 60  0001 C CNN
F 4 "497-14647-ND" H 0   0   50  0001 C CNN "Digikey PN"
	1    4950 3150
	1    0    0    -1  
$EndComp
Text HLabel 4050 3050 0    50   Output ~ 0
DIR_A
Text HLabel 6000 2900 2    50   Output ~ 0
DIR_B
Text HLabel 7000 2900 2    50   Output ~ 0
CEN_A
Text HLabel 7000 2950 2    50   Output ~ 0
DEN_A
Text HLabel 5550 3650 2    50   Output ~ 0
CEN_B
Wire Wire Line
	3600 3850 3600 4000
Wire Wire Line
	4000 3850 4000 4000
Wire Wire Line
	3150 3700 2900 3700
Wire Wire Line
	3150 3250 3150 3700
Connection ~ 2900 3700
Text HLabel 2650 3650 0    50   Output ~ 0
DEN_B
Wire Wire Line
	2650 3650 2900 3650
Connection ~ 2900 3650
Wire Wire Line
	5550 3650 5450 3650
Text Label 4800 6300 0    50   ~ 0
DIR_A
Text Label 4800 6200 0    50   ~ 0
DIR_B
Text HLabel 5850 6200 2    50   Output ~ 0
nDIR_B
Text HLabel 5850 6300 2    50   Output ~ 0
nDIR_A
Wire Wire Line
	4800 6200 5100 6200
Wire Wire Line
	4800 6300 5100 6300
Wire Wire Line
	5850 6200 5600 6200
Wire Wire Line
	5600 6300 5850 6300
Text Label 4150 3050 0    50   ~ 0
DIR_A
Wire Wire Line
	3750 2950 4400 2950
Wire Wire Line
	4050 3050 4450 3050
Wire Wire Line
	3750 2950 3750 3050
Wire Wire Line
	4400 2950 4400 3000
Wire Wire Line
	4400 3000 4450 3000
Text Label 5750 2900 0    50   ~ 0
DIR_B
Wire Wire Line
	5450 2900 6000 2900
Wire Wire Line
	6350 2950 6350 1900
Wire Wire Line
	5450 2950 6350 2950
Wire Wire Line
	6350 1900 7400 1900
Wire Wire Line
	6400 2000 6400 3000
Wire Wire Line
	5450 3000 6400 3000
Wire Wire Line
	6400 2000 6700 2000
Wire Wire Line
	6500 3050 6500 2900
Wire Wire Line
	5450 3050 6500 3050
Wire Wire Line
	6550 2950 6550 3100
Wire Wire Line
	5450 3100 6550 3100
Wire Wire Line
	6650 3200 6650 3050
Wire Wire Line
	5450 3200 6650 3200
Wire Wire Line
	6700 3100 6700 3250
Wire Wire Line
	5450 3250 6700 3250
Wire Wire Line
	6800 3300 6800 3200
Wire Wire Line
	6800 3200 7000 3200
Wire Wire Line
	5450 3300 6800 3300
Wire Wire Line
	7000 3250 6850 3250
Wire Wire Line
	6850 3250 6850 3350
Wire Wire Line
	5450 3350 6850 3350
Wire Wire Line
	6700 3100 7000 3100
Wire Wire Line
	6650 3050 7000 3050
Wire Wire Line
	6550 2950 7000 2950
Wire Wire Line
	6500 2900 7000 2900
$Comp
L local:74LVC2G04GW,125 U3
U 1 1 5BB542CB
P 5350 6200
F 0 "U3" H 5350 6515 50  0000 C CNN
F 1 "74LVC2G04GW,125" H 5350 6424 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-363_SC-70-6" H 5350 6200 50  0001 C CNN
F 3 "" H 5350 6200 50  0001 C CNN
F 4 "1727-4032-1-ND" H 0   0   50  0001 C CNN "Digikey PN"
	1    5350 6200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0133
U 1 1 5BB71B04
P 5600 6500
F 0 "#PWR0133" H 5600 6250 50  0001 C CNN
F 1 "GND" H 5605 6327 50  0000 C CNN
F 2 "" H 5600 6500 50  0001 C CNN
F 3 "" H 5600 6500 50  0001 C CNN
	1    5600 6500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 6400 5600 6500
$Comp
L power:+3.3V #PWR0134
U 1 1 5BB7BA72
P 4350 5900
F 0 "#PWR0134" H 4350 5750 50  0001 C CNN
F 1 "+3.3V" H 4365 6073 50  0000 C CNN
F 2 "" H 4350 5900 50  0001 C CNN
F 3 "" H 4350 5900 50  0001 C CNN
	1    4350 5900
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C10
U 1 1 5BB7BACF
P 4350 6200
F 0 "C10" H 4400 6300 50  0000 L CNN
F 1 "100n" H 4400 6150 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4350 6200 50  0001 C CNN
F 3 "~" H 4350 6200 50  0001 C CNN
	1    4350 6200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0135
U 1 1 5BB7BB73
P 4350 6500
F 0 "#PWR0135" H 4350 6250 50  0001 C CNN
F 1 "GND" H 4355 6327 50  0000 C CNN
F 2 "" H 4350 6500 50  0001 C CNN
F 3 "" H 4350 6500 50  0001 C CNN
	1    4350 6500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 6500 4350 6300
Wire Wire Line
	4350 5900 4350 6000
Wire Wire Line
	4350 6000 5100 6000
Wire Wire Line
	5100 6000 5100 6100
Wire Wire Line
	4350 6000 4350 6100
Connection ~ 4350 6000
Wire Wire Line
	2550 5300 2550 5350
Connection ~ 2550 5350
Wire Wire Line
	2600 5350 2600 5300
Wire Wire Line
	2550 5350 2600 5350
Connection ~ 2600 5350
Wire Wire Line
	2600 5350 2650 5350
Wire Wire Line
	2650 5300 2650 5350
Connection ~ 2650 5350
Wire Wire Line
	2650 5350 2700 5350
Wire Wire Line
	3050 5100 3050 5550
Wire Wire Line
	3050 5000 3200 5000
$EndSCHEMATC
