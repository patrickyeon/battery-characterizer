EESchema Schematic File Version 4
LIBS:characterizer-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 8
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	4600 5900 4600 5950
Wire Wire Line
	5950 5900 5050 5900
Wire Wire Line
	5950 2800 5950 5900
Wire Wire Line
	6300 2800 5950 2800
Wire Wire Line
	4450 2700 4450 2750
Wire Wire Line
	6300 2700 5050 2700
Wire Wire Line
	2850 2750 2850 5850
Wire Wire Line
	2500 2750 2850 2750
Wire Wire Line
	4500 6350 4400 6350
Wire Wire Line
	4500 6400 4500 6350
Wire Wire Line
	4600 6350 4600 6400
Wire Wire Line
	4400 6350 4400 6400
Connection ~ 4500 6350
Wire Wire Line
	4600 6350 4500 6350
Wire Wire Line
	4600 6400 4800 6400
Connection ~ 4600 6350
Wire Wire Line
	4600 6250 4600 6350
Connection ~ 4600 5900
Wire Wire Line
	4600 5850 4600 5900
Connection ~ 4600 6400
$Comp
L Connector_Generic:Conn_01x03 J?
U 1 1 5BBBF85D
P 4500 6600
AR Path="/5BBBF85D" Ref="J?"  Part="1" 
AR Path="/5BBBE9BB/5BBBF85D" Ref="J?"  Part="1" 
AR Path="/5BBD093B/5BBBF85D" Ref="J7"  Part="1" 
AR Path="/5BC4835C/5BBBF85D" Ref="J11"  Part="1" 
F 0 "J7" V 4350 6250 50  0000 L CNN
F 1 "36-54-ND" V 4450 5950 50  0000 L CNN
F 2 "project:BATTERY_CLIP_36-54" H 4500 6600 50  0001 C CNN
F 3 "~" H 4500 6600 50  0001 C CNN
F 4 "36-54-ND" H 0   0   50  0001 C CNN "Digikey PN"
	1    4500 6600
	0    1    1    0   
$EndComp
Connection ~ 4600 5850
Wire Wire Line
	4500 5850 4600 5850
Connection ~ 4500 5850
Wire Wire Line
	2850 5850 4400 5850
Wire Wire Line
	4400 5850 4500 5850
Connection ~ 4400 5850
$Comp
L Connector_Generic:Conn_01x03 J?
U 1 1 5BBBF86A
P 4500 5650
AR Path="/5BBBF86A" Ref="J?"  Part="1" 
AR Path="/5BBBE9BB/5BBBF86A" Ref="J?"  Part="1" 
AR Path="/5BBD093B/5BBBF86A" Ref="J6"  Part="1" 
AR Path="/5BC4835C/5BBBF86A" Ref="J10"  Part="1" 
F 0 "J6" V 4466 5462 50  0000 R CNN
F 1 "36-54-ND" V 4375 5462 50  0000 R CNN
F 2 "project:BATTERY_CLIP_36-54" H 4500 5650 50  0001 C CNN
F 3 "~" H 4500 5650 50  0001 C CNN
F 4 "36-54-ND" H 0   0   50  0001 C CNN "Digikey PN"
	1    4500 5650
	0    -1   -1   0   
$EndComp
$Comp
L Device:Battery_Cell BT?
U 1 1 5BBBF877
P 4600 6150
AR Path="/5BBBF877" Ref="BT?"  Part="1" 
AR Path="/5BBBE9BB/5BBBF877" Ref="BT?"  Part="1" 
AR Path="/5BBD093B/5BBBF877" Ref="BT2"  Part="1" 
AR Path="/5BC4835C/5BBBF877" Ref="BT4"  Part="1" 
F 0 "BT2" H 4718 6246 50  0000 L CNN
F 1 "Battery_Cell" H 4718 6155 50  0000 L CNN
F 2 "project:18650-keyelco" V 4600 6210 50  0001 C CNN
F 3 "~" V 4600 6210 50  0001 C CNN
	1    4600 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 3150 4250 3150
Wire Wire Line
	4350 3200 4350 3150
Wire Wire Line
	4450 3150 4450 3200
Wire Wire Line
	4250 3150 4250 3200
Connection ~ 4350 3150
Wire Wire Line
	4450 3150 4350 3150
Wire Wire Line
	4450 3200 4650 3200
Connection ~ 4450 3150
Wire Wire Line
	4450 3050 4450 3150
Connection ~ 4450 2700
Wire Wire Line
	4450 2650 4450 2700
Connection ~ 4450 3200
$Comp
L Connector_Generic:Conn_01x03 J?
U 1 1 5BBBF88B
P 4350 3400
AR Path="/5BBBF88B" Ref="J?"  Part="1" 
AR Path="/5BBBE9BB/5BBBF88B" Ref="J?"  Part="1" 
AR Path="/5BBD093B/5BBBF88B" Ref="J5"  Part="1" 
AR Path="/5BC4835C/5BBBF88B" Ref="J9"  Part="1" 
F 0 "J5" V 4200 3050 50  0000 L CNN
F 1 "36-54-ND" V 4300 2750 50  0000 L CNN
F 2 "project:BATTERY_CLIP_36-54" H 4350 3400 50  0001 C CNN
F 3 "~" H 4350 3400 50  0001 C CNN
F 4 "36-54-ND" H 0   0   50  0001 C CNN "Digikey PN"
	1    4350 3400
	0    1    1    0   
$EndComp
Connection ~ 4450 2650
Wire Wire Line
	4350 2650 4450 2650
Connection ~ 4350 2650
Wire Wire Line
	4250 2650 2500 2650
Wire Wire Line
	4250 2650 4350 2650
Connection ~ 4250 2650
$Comp
L Connector_Generic:Conn_01x03 J?
U 1 1 5BBBF898
P 4350 2450
AR Path="/5BBBF898" Ref="J?"  Part="1" 
AR Path="/5BBBE9BB/5BBBF898" Ref="J?"  Part="1" 
AR Path="/5BBD093B/5BBBF898" Ref="J4"  Part="1" 
AR Path="/5BC4835C/5BBBF898" Ref="J8"  Part="1" 
F 0 "J4" V 4316 2262 50  0000 R CNN
F 1 "36-54-ND" V 4225 2262 50  0000 R CNN
F 2 "project:BATTERY_CLIP_36-54" H 4350 2450 50  0001 C CNN
F 3 "~" H 4350 2450 50  0001 C CNN
F 4 "36-54-ND" H 0   0   50  0001 C CNN "Digikey PN"
	1    4350 2450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4750 4750 4750 4950
Wire Wire Line
	4750 4500 4750 4550
$Comp
L power:+3.3V #PWR?
U 1 1 5BBBF8A1
P 4750 4500
AR Path="/5BBBF8A1" Ref="#PWR?"  Part="1" 
AR Path="/5BBBE9BB/5BBBF8A1" Ref="#PWR?"  Part="1" 
AR Path="/5BBD093B/5BBBF8A1" Ref="#PWR0180"  Part="1" 
AR Path="/5BC4835C/5BBBF8A1" Ref="#PWR0136"  Part="1" 
F 0 "#PWR0180" H 4750 4350 50  0001 C CNN
F 1 "+3.3V" H 4765 4673 50  0000 C CNN
F 2 "" H 4750 4500 50  0001 C CNN
F 3 "" H 4750 4500 50  0001 C CNN
	1    4750 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 4500 3500 4700
Wire Wire Line
	3900 4900 3700 4900
Wire Wire Line
	3700 4950 3900 4950
Text Label 3700 4950 0    50   ~ 0
SCL
Text Label 3700 4900 0    50   ~ 0
SDA
Wire Wire Line
	3900 4700 3900 4800
Wire Wire Line
	3500 4700 3900 4700
Wire Wire Line
	3500 5200 3500 4900
$Comp
L power:GND #PWR?
U 1 1 5BBBF8AF
P 3500 5200
AR Path="/5BBBF8AF" Ref="#PWR?"  Part="1" 
AR Path="/5BBBE9BB/5BBBF8AF" Ref="#PWR?"  Part="1" 
AR Path="/5BBD093B/5BBBF8AF" Ref="#PWR0181"  Part="1" 
AR Path="/5BC4835C/5BBBF8AF" Ref="#PWR0137"  Part="1" 
F 0 "#PWR0181" H 3500 4950 50  0001 C CNN
F 1 "GND" H 3505 5027 50  0000 C CNN
F 2 "" H 3500 5200 50  0001 C CNN
F 3 "" H 3500 5200 50  0001 C CNN
	1    3500 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 4850 5300 4850
Wire Wire Line
	5050 4900 5050 4950
Wire Wire Line
	4600 4900 5050 4900
Wire Wire Line
	4600 4950 4750 4950
Wire Wire Line
	4600 5150 4600 5200
Connection ~ 4600 5150
Wire Wire Line
	4600 5050 4600 5150
$Comp
L power:GND #PWR?
U 1 1 5BBBF8BD
P 4600 5200
AR Path="/5BBBF8BD" Ref="#PWR?"  Part="1" 
AR Path="/5BBBE9BB/5BBBF8BD" Ref="#PWR?"  Part="1" 
AR Path="/5BBD093B/5BBBF8BD" Ref="#PWR0182"  Part="1" 
AR Path="/5BC4835C/5BBBF8BD" Ref="#PWR0138"  Part="1" 
F 0 "#PWR0182" H 4600 4950 50  0001 C CNN
F 1 "GND" H 4605 5027 50  0000 C CNN
F 2 "" H 4600 5200 50  0001 C CNN
F 3 "" H 4600 5200 50  0001 C CNN
	1    4600 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 5150 5050 5150
$Comp
L Device:R_Small R?
U 1 1 5BBBF8CD
P 5050 5050
AR Path="/5BBBF8CD" Ref="R?"  Part="1" 
AR Path="/5BBBE9BB/5BBBF8CD" Ref="R?"  Part="1" 
AR Path="/5BBD093B/5BBBF8CD" Ref="R28"  Part="1" 
AR Path="/5BC4835C/5BBBF8CD" Ref="R65"  Part="1" 
F 0 "R28" H 5109 5096 50  0000 L CNN
F 1 "10k" H 5109 5005 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 5050 5050 50  0001 C CNN
F 3 "~" H 5050 5050 50  0001 C CNN
	1    5050 5050
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 5BBBF8D4
P 4750 4650
AR Path="/5BBBF8D4" Ref="R?"  Part="1" 
AR Path="/5BBBE9BB/5BBBF8D4" Ref="R?"  Part="1" 
AR Path="/5BBD093B/5BBBF8D4" Ref="R26"  Part="1" 
AR Path="/5BC4835C/5BBBF8D4" Ref="R63"  Part="1" 
F 0 "R26" H 4809 4696 50  0000 L CNN
F 1 "10k" H 4809 4605 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 4750 4650 50  0001 C CNN
F 3 "~" H 4750 4650 50  0001 C CNN
	1    4750 4650
	1    0    0    -1  
$EndComp
NoConn ~ 3900 5050
Connection ~ 3500 4700
$Comp
L Device:C_Small C?
U 1 1 5BBBF8DD
P 3500 4800
AR Path="/5BBBF8DD" Ref="C?"  Part="1" 
AR Path="/5BBBE9BB/5BBBF8DD" Ref="C?"  Part="1" 
AR Path="/5BBD093B/5BBBF8DD" Ref="C16"  Part="1" 
AR Path="/5BC4835C/5BBBF8DD" Ref="C33"  Part="1" 
F 0 "C16" H 3592 4846 50  0000 L CNN
F 1 "100n" H 3592 4755 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3500 4800 50  0001 C CNN
F 3 "~" H 3500 4800 50  0001 C CNN
	1    3500 4800
	-1   0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5BBBF8E4
P 3500 4500
AR Path="/5BBBF8E4" Ref="#PWR?"  Part="1" 
AR Path="/5BBBE9BB/5BBBF8E4" Ref="#PWR?"  Part="1" 
AR Path="/5BBD093B/5BBBF8E4" Ref="#PWR0183"  Part="1" 
AR Path="/5BC4835C/5BBBF8E4" Ref="#PWR0139"  Part="1" 
F 0 "#PWR0183" H 3500 4350 50  0001 C CNN
F 1 "+3.3V" H 3515 4673 50  0000 C CNN
F 2 "" H 3500 4500 50  0001 C CNN
F 3 "" H 3500 4500 50  0001 C CNN
	1    3500 4500
	1    0    0    -1  
$EndComp
$Comp
L local:AT30TS74 U?
U 1 1 5BBBF8EB
P 4250 4900
AR Path="/5BBBF8EB" Ref="U?"  Part="1" 
AR Path="/5BBBE9BB/5BBBF8EB" Ref="U?"  Part="1" 
AR Path="/5BBD093B/5BBBF8EB" Ref="U5"  Part="1" 
AR Path="/5BC4835C/5BBBF8EB" Ref="U10"  Part="1" 
F 0 "U5" H 4250 5215 50  0000 C CNN
F 1 "AT30TS74" H 4250 5124 50  0000 C CNN
F 2 "Package_SO:MSOP-8_3x3mm_P0.65mm" H 4250 4900 50  0001 C CNN
F 3 "" H 4250 4900 50  0001 C CNN
F 4 "1611-AT30TS74-XM8M-B-ND" H 4250 5123 50  0001 C CNN "Digikey PN"
	1    4250 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 1750 3550 1750
Wire Wire Line
	3550 1800 3750 1800
Text Label 3550 1800 0    50   ~ 0
SCL
Text Label 3550 1750 0    50   ~ 0
SDA
Wire Wire Line
	3750 1550 3750 1650
Wire Wire Line
	3050 1500 3050 1550
Wire Wire Line
	3050 2050 3050 1750
$Comp
L power:GND #PWR?
U 1 1 5BBBF8FA
P 3050 2050
AR Path="/5BBBF8FA" Ref="#PWR?"  Part="1" 
AR Path="/5BBBE9BB/5BBBF8FA" Ref="#PWR?"  Part="1" 
AR Path="/5BBD093B/5BBBF8FA" Ref="#PWR0184"  Part="1" 
AR Path="/5BC4835C/5BBBF8FA" Ref="#PWR0140"  Part="1" 
F 0 "#PWR0184" H 3050 1800 50  0001 C CNN
F 1 "GND" H 3055 1877 50  0000 C CNN
F 2 "" H 3050 2050 50  0001 C CNN
F 3 "" H 3050 2050 50  0001 C CNN
	1    3050 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 1700 5150 1700
Wire Wire Line
	4450 1800 4600 1800
Wire Wire Line
	4450 2000 4450 2050
Connection ~ 4450 2000
Wire Wire Line
	4450 1900 4450 2000
$Comp
L power:GND #PWR?
U 1 1 5BBBF908
P 4450 2050
AR Path="/5BBBF908" Ref="#PWR?"  Part="1" 
AR Path="/5BBBE9BB/5BBBF908" Ref="#PWR?"  Part="1" 
AR Path="/5BBD093B/5BBBF908" Ref="#PWR0185"  Part="1" 
AR Path="/5BC4835C/5BBBF908" Ref="#PWR0141"  Part="1" 
F 0 "#PWR0185" H 4450 1800 50  0001 C CNN
F 1 "GND" H 4455 1877 50  0000 C CNN
F 2 "" H 4450 2050 50  0001 C CNN
F 3 "" H 4450 2050 50  0001 C CNN
	1    4450 2050
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 5BBBF90E
P 4850 1900
AR Path="/5BBBF90E" Ref="R?"  Part="1" 
AR Path="/5BBBE9BB/5BBBF90E" Ref="R?"  Part="1" 
AR Path="/5BBD093B/5BBBF90E" Ref="R27"  Part="1" 
AR Path="/5BC4835C/5BBBF90E" Ref="R64"  Part="1" 
F 0 "R27" H 4909 1946 50  0000 L CNN
F 1 "10k" H 4909 1855 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 4850 1900 50  0001 C CNN
F 3 "~" H 4850 1900 50  0001 C CNN
	1    4850 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 2000 4450 2000
Connection ~ 4600 2000
$Comp
L Device:R_Small R?
U 1 1 5BBBF921
P 4600 1900
AR Path="/5BBBF921" Ref="R?"  Part="1" 
AR Path="/5BBBE9BB/5BBBF921" Ref="R?"  Part="1" 
AR Path="/5BBD093B/5BBBF921" Ref="R25"  Part="1" 
AR Path="/5BC4835C/5BBBF921" Ref="R62"  Part="1" 
F 0 "R25" H 4659 1946 50  0000 L CNN
F 1 "10k" H 4659 1855 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 4600 1900 50  0001 C CNN
F 3 "~" H 4600 1900 50  0001 C CNN
	1    4600 1900
	1    0    0    -1  
$EndComp
NoConn ~ 3750 1900
Connection ~ 3050 1550
$Comp
L Device:C_Small C?
U 1 1 5BBBF92A
P 3050 1650
AR Path="/5BBBF92A" Ref="C?"  Part="1" 
AR Path="/5BBBE9BB/5BBBF92A" Ref="C?"  Part="1" 
AR Path="/5BBD093B/5BBBF92A" Ref="C15"  Part="1" 
AR Path="/5BC4835C/5BBBF92A" Ref="C32"  Part="1" 
F 0 "C15" H 3142 1696 50  0000 L CNN
F 1 "100n" H 3142 1605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3050 1650 50  0001 C CNN
F 3 "~" H 3050 1650 50  0001 C CNN
	1    3050 1650
	-1   0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5BBBF931
P 3050 1500
AR Path="/5BBBF931" Ref="#PWR?"  Part="1" 
AR Path="/5BBBE9BB/5BBBF931" Ref="#PWR?"  Part="1" 
AR Path="/5BBD093B/5BBBF931" Ref="#PWR0186"  Part="1" 
AR Path="/5BC4835C/5BBBF931" Ref="#PWR0142"  Part="1" 
F 0 "#PWR0186" H 3050 1350 50  0001 C CNN
F 1 "+3.3V" H 3065 1673 50  0000 C CNN
F 2 "" H 3050 1500 50  0001 C CNN
F 3 "" H 3050 1500 50  0001 C CNN
	1    3050 1500
	1    0    0    -1  
$EndComp
$Comp
L local:AT30TS74 U?
U 1 1 5BBBF938
P 4100 1750
AR Path="/5BBBF938" Ref="U?"  Part="1" 
AR Path="/5BBBE9BB/5BBBF938" Ref="U?"  Part="1" 
AR Path="/5BBD093B/5BBBF938" Ref="U4"  Part="1" 
AR Path="/5BC4835C/5BBBF938" Ref="U9"  Part="1" 
F 0 "U4" H 4100 2065 50  0000 C CNN
F 1 "AT30TS74" H 4100 1974 50  0000 C CNN
F 2 "Package_SO:MSOP-8_3x3mm_P0.65mm" H 4100 1750 50  0001 C CNN
F 3 "" H 4100 1750 50  0001 C CNN
F 4 "1611-AT30TS74-XM8M-B-ND" H 4100 1973 50  0001 C CNN "Digikey PN"
	1    4100 1750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5BBBF93F
P 4650 3450
AR Path="/5BBBF93F" Ref="#PWR?"  Part="1" 
AR Path="/5BBBE9BB/5BBBF93F" Ref="#PWR?"  Part="1" 
AR Path="/5BBD093B/5BBBF93F" Ref="#PWR0187"  Part="1" 
AR Path="/5BC4835C/5BBBF93F" Ref="#PWR0143"  Part="1" 
F 0 "#PWR0187" H 4650 3200 50  0001 C CNN
F 1 "GND" H 4655 3277 50  0000 C CNN
F 2 "" H 4650 3450 50  0001 C CNN
F 3 "" H 4650 3450 50  0001 C CNN
	1    4650 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:Battery_Cell BT?
U 1 1 5BBBF945
P 4450 2950
AR Path="/5BBBF945" Ref="BT?"  Part="1" 
AR Path="/5BBBE9BB/5BBBF945" Ref="BT?"  Part="1" 
AR Path="/5BBD093B/5BBBF945" Ref="BT1"  Part="1" 
AR Path="/5BC4835C/5BBBF945" Ref="BT3"  Part="1" 
F 0 "BT1" H 4568 3046 50  0000 L CNN
F 1 "Battery_Cell" H 4568 2955 50  0000 L CNN
F 2 "project:18650-keyelco" V 4450 3010 50  0001 C CNN
F 3 "~" V 4450 3010 50  0001 C CNN
	1    4450 2950
	1    0    0    -1  
$EndComp
$Sheet
S 6300 2450 750  650 
U 5BBBF94F
F0 "sheet5BBBF841" 50
F1 "load.sch" 50
F2 "DCHG_EN" I R 7050 2550 50 
F3 "Viset" I R 7050 2900 50 
F4 "Vimeas" O R 7050 3000 50 
F5 "Vbatt_0" I L 6300 2700 50 
F6 "Vbatt_1" I L 6300 2800 50 
F7 "DIR" I R 7050 2650 50 
F8 "nDIR" I R 7050 2750 50 
$EndSheet
$Sheet
S 1750 2500 750  650 
U 5BBBF958
F0 "sheet5BBBF842" 50
F1 "charger.sch" 50
F2 "EN" I L 1750 2600 50 
F3 "Viset" I L 1750 3000 50 
F4 "Vimeas" O L 1750 3100 50 
F5 "Vbatt_0" O R 2500 2650 50 
F6 "Vbatt_1" O R 2500 2750 50 
F7 "DIR" I L 1750 2700 50 
F8 "nDIR" I L 1750 2800 50 
$EndSheet
Wire Wire Line
	4600 2000 4850 2000
Text HLabel 5150 1700 2    50   Input ~ 0
ADDR
Text Label 4950 1700 0    50   ~ 0
ADDR
Wire Wire Line
	4850 1750 4850 1800
Wire Wire Line
	4450 1750 4850 1750
Text Label 5100 4850 0    50   ~ 0
ADDR
Text HLabel 1400 2700 0    50   Input ~ 0
DIR
Text HLabel 1400 2800 0    50   Input ~ 0
nDIR
Text HLabel 1400 2600 0    50   Input ~ 0
CHARGE
Text HLabel 1400 3000 0    50   Input ~ 0
Vi_chrg_set
Text HLabel 1400 3100 0    50   Output ~ 0
Vi_chrg_meas
Text Label 1500 2700 0    50   ~ 0
DIR
Text Label 1500 2800 0    50   ~ 0
nDIR
Text HLabel 7350 2550 2    50   Input ~ 0
DCHARGE
Text Label 7150 2650 0    50   ~ 0
DIR
Text Label 7150 2750 0    50   ~ 0
nDIR
Text HLabel 7350 2900 2    50   Input ~ 0
Vi_dchrg_set
Text HLabel 7350 3000 2    50   Output ~ 0
Vi_dchrg_meas
Wire Wire Line
	1400 2600 1750 2600
Wire Wire Line
	1750 2700 1400 2700
Wire Wire Line
	1400 2800 1750 2800
Wire Wire Line
	1750 3000 1400 3000
Wire Wire Line
	1400 3100 1750 3100
Wire Wire Line
	7050 2550 7350 2550
Wire Wire Line
	7350 2650 7050 2650
Wire Wire Line
	7350 2750 7050 2750
Wire Wire Line
	7350 2900 7050 2900
Wire Wire Line
	7050 3000 7350 3000
Wire Wire Line
	3050 1550 3750 1550
Text HLabel 3550 1750 0    50   BiDi ~ 0
SDA
Text HLabel 3550 1800 0    50   Input ~ 0
SCL
Text HLabel 5150 2800 2    50   Output ~ 0
Vbatt_0
Text HLabel 5150 6000 2    50   Output ~ 0
Vbatt_1
Wire Wire Line
	5150 6000 5050 6000
Wire Wire Line
	5050 6000 5050 5900
Connection ~ 5050 5900
Wire Wire Line
	5050 5900 4600 5900
Wire Wire Line
	5150 2800 5050 2800
Wire Wire Line
	5050 2800 5050 2700
Connection ~ 5050 2700
Wire Wire Line
	5050 2700 4450 2700
$Comp
L Device:Polyfuse_Small F1
U 1 1 5BBF46E7
P 4650 3300
AR Path="/5BBD093B/5BBF46E7" Ref="F1"  Part="1" 
AR Path="/5BC4835C/5BBF46E7" Ref="F3"  Part="1" 
F 0 "F1" H 4718 3346 50  0000 L CNN
F 1 "MF-USML700-2" H 4718 3255 50  0000 L CNN
F 2 "Resistor_SMD:R_1210_3225Metric" H 4700 3100 50  0001 L CNN
F 3 "~" H 4650 3300 50  0001 C CNN
F 4 "MF-USML700-2CT-ND" H 4650 3300 50  0001 C CNN "Digikey PN"
	1    4650 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4650 3450 4650 3400
$Comp
L power:GND #PWR?
U 1 1 5BBF6372
P 4800 6650
AR Path="/5BBF6372" Ref="#PWR?"  Part="1" 
AR Path="/5BBBE9BB/5BBF6372" Ref="#PWR?"  Part="1" 
AR Path="/5BBD093B/5BBF6372" Ref="#PWR0188"  Part="1" 
AR Path="/5BC4835C/5BBF6372" Ref="#PWR0144"  Part="1" 
F 0 "#PWR0188" H 4800 6400 50  0001 C CNN
F 1 "GND" H 4805 6477 50  0000 C CNN
F 2 "" H 4800 6650 50  0001 C CNN
F 3 "" H 4800 6650 50  0001 C CNN
	1    4800 6650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 6650 4800 6600
$Comp
L Device:Polyfuse_Small F2
U 1 1 5BBF7E56
P 4800 6500
AR Path="/5BBD093B/5BBF7E56" Ref="F2"  Part="1" 
AR Path="/5BC4835C/5BBF7E56" Ref="F4"  Part="1" 
F 0 "F2" H 4868 6546 50  0000 L CNN
F 1 "MF-USML700-2" H 4868 6455 50  0000 L CNN
F 2 "Resistor_SMD:R_1210_3225Metric" H 4850 6300 50  0001 L CNN
F 3 "~" H 4800 6500 50  0001 C CNN
F 4 "MF-USML700-2CT-ND" H 4800 6500 50  0001 C CNN "Digikey PN"
	1    4800 6500
	1    0    0    -1  
$EndComp
$EndSCHEMATC
