// !-------------------------------------------------------------------------!
// ! Prodave6.h       PRODAVE S7 V6 Header-Datei                             !
// !-------------------------------------------------------------------------!
// ! Created:      10.09.04                                                  !
// !                                                                         !
// ! Last Update:                                                            !
// !                                                                         !
// !                                                                         !
// !                                                                         !
// !                                                                         !
// !-------------------------------------------------------------------------!
// ! Copyright (C) Siemens AG 2004  All Rights Reserved. Confidential        !
// !-------------------------------------------------------------------------!
#ifndef _PRODAVE6_H_
#define _PRODAVE6_H_

#pragma pack(8)        // set packing for Prodave6.DLL


// ---------------------------------------------------------------------------
// Includes
// ---------------------------------------------------------------------------
#include <time.h>


// ---------------------------------------------------------------------------
// Base defines
// ---------------------------------------------------------------------------
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

// ---------------------------------------------------------------------------
// PRODAVE6 defines
// ---------------------------------------------------------------------------
#define MAX_CONNECTION			64			// Maximale Anzahl Verbindungen 64
#define ERROR_MSG_LEN			256			// L鋘ge f黵 ErrorMassages 256
#define MAX_ERROR_ENTRIES		512			// Maximale Anzahl ErrorMassages 256
#define MAX_DEVNAME_LEN			128			// AccessPoint z.B.: "S7ONLINE"
#define MAX_BUFFER				65536		// Maximale Puffergr鲞e
#define LITTLE_BUFFER			256			// Normale Puffergr鲞e f黵 kleinere Zugriffe
#define MAX_BLK					1024		// Maximale Anzahl Bausteine
#define SIZE_BOOL_BYTE			8			// 8 Byte f黵 Array in bool_2_byte...
#define MUTEX_TIMEOUT			0xFFFFFFFF	// Wait max. for 5 sec.
#define WTS_USERID_LEN			15			// Stringlaenge der von der AS gesendeten UserID (ohne \0 !)
#define IDENTIFY_MAX_BUFFER		1024*sizeof(CD7NODESTRUCT)		// Puffergr鲞e NUR f黵 die Ethernet LifeList

// ---------------------------------------------------------------------------
// Base data types
// ---------------------------------------------------------------------------
// BASETYPES is defined in <windows.h> or <windef.h> and can be redefined
// in Prodave6.h
#ifndef BASETYPES
#define BASETYPES
typedef unsigned long	ULONG;		// unsigned 32 Bit value
typedef ULONG *PULONG;
typedef unsigned short	USHORT;		// unsigned 16 Bit value
typedef USHORT *PUSHORT;
typedef unsigned char	UCHAR;		// unsigned 8 Bit value
typedef UCHAR *PUCHAR;
typedef char *PSZ;
typedef int			BOOL;
#endif


// ---------------------------------------------------------------------------
// PRODAVE6 structure types
// ---------------------------------------------------------------------------
#pragma pack(push, 1)  // push, set strikt packing

typedef unsigned char TTIMESTAMP[6];//Zeitstempel

#ifndef TS_USERID
	typedef struct {				// Spezifikation der UserID
		UCHAR Data[WTS_USERID_LEN+1];
	} TS_USERID;
#endif

typedef union {
	unsigned char Mpi;				// MPI Stationsadresse	(2)
	unsigned char Ip[4];			// IP Adresse			(192.168.0.1)
	unsigned char Mac[6];			// MAC Adresse			(08-00-06-01-AA-BB)
} CON_ADR_TYPE;

typedef struct {
	CON_ADR_TYPE Adr;				// Verbindungsadresse
	unsigned char AdrType;			// Typ der Adresse MPI(1) IP(2) MAC(3)
	unsigned char SlotNr;			// Slot-Nummer
	unsigned char RackNr;			// Rack-Nummer
} CON_TABLE_TYPE;

#pragma pack(pop)  // pop, set packing for Prodave6.DLL  

typedef struct {
	unsigned char Plcas[4];			// Ausgabestand PLC
	unsigned char Pgas[2];			// Ausgabestand PGAS
	char Mlfb[20];					// MLFB der angeschlossenen AS
} AS_INFO_TYPE;

typedef struct {
	unsigned long Firmware[4];
	unsigned long ASIC[4];
	char Mlfb[16];			// MLFB der angeschlossenen AS200
} AS200_INFO_TYPE;

typedef struct {
	unsigned short BlkNr;			// Bausteinnummer
	struct tm TimeStamp;			// Zeitstempel zu der Bausteinnummer
} BST_DIAG_TYPE;

typedef struct {
	unsigned short EventID;			// EreignisID
	unsigned char EventInfo[10];	// Info zum Ereignis
	unsigned char Timestamp[8];		// Zeitstempel des Ereignises
} DIAG_BUFFER_TYPE;

typedef struct {
	unsigned short BlkType;			// Bausteintyp
	unsigned short BlkNumber;		// Bausteinnummer
	char BlkName[8];				// Bausteinname
	unsigned char BlkVersion;		// Bausteinversion
	unsigned long BlkLength;		// Bausteinl鋘ge
	struct tm BlkTimestamp1;		// Zeitstempel zu der Bausteinnummer
	struct tm BlkTimestamp2;		// Zeitstempel zu der Bausteinnummer
	unsigned char blkSecurity[4];	// Baustein Schutzstufe
} BST_STAT_TYPE;

typedef struct {
	unsigned short ProgLang;		// Programiersprache
	unsigned short BlkType;			// Bausteintyp
	unsigned short BlkNumber;		// Bausteinnummer
	unsigned char Attribute;		// Attribute
	unsigned char BlkVersion;		// Bausteinversion
	unsigned long BlkLength;		// Bausteinl鋘ge
	unsigned long Length[3];		// Datenl鋘ge
	unsigned short DynLen;			// L鋘ge der dyn. lokal Daten
	struct tm BlkTimestamp1;		// Zeitstempel zu der Bausteinnummer
	struct tm BlkTimestamp2;		// Zeitstempel zu der Bausteinnummer
	unsigned char blkSecurity[4];	// Baustein  Schutzstufe
	unsigned char ProducerName[8];	// Producer Name
	unsigned char BlkFamName[8];	// Baustein Familien Name
	unsigned char BlkName[8];		// Bausteinname
	unsigned char Version;			// Dies ist eine Anwenderversion und hat nicht mit der Bausteinversion zu tun
	unsigned char res1;				// F黮lbyte
	unsigned short chckSum;			// Checksumme
	unsigned long CPUType;			// CPU-Typ
	unsigned short signature;		// signature
	unsigned short res2;			// F黮lbytes
} BST_HEADER_TYPE;


typedef struct {
	unsigned char	MacAddress[6];
	unsigned char	IPAdress[4];
	unsigned char	LengthType;
	unsigned char	res1;
	unsigned char	TypeOfStation[20];
	unsigned char	LengthName;
	unsigned char	res2;
	unsigned char	NameOfStation[255];
	unsigned char	res3;
} IE_NODE;


// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// PRODAVE6 functions
// ---------------------------------------------------------------------------
#ifdef OS_WINDOWS
#define PRODAVEAPI  __stdcall // use the Win32-API definition  WINAPI
#else
# define PRODAVEAPI
#endif


#ifdef __cplusplus  // used with C++
extern "C" {
#endif

int PRODAVEAPI LoadConnection_ex6 (unsigned short ConNr, char* pAccessPoint, unsigned short ConTableLen, CON_TABLE_TYPE * pConTable);
typedef int ( PRODAVEAPI *pfn_LoadConnection_ex6 ) (
        unsigned short ConNr,
        char* pAccessPoint,
        unsigned short ConTableLen,
        CON_TABLE_TYPE * pConTable
    );

int PRODAVEAPI UnloadConnection_ex6 (unsigned short ConNr);
typedef int ( PRODAVEAPI *pfn_UnloadConnection_ex6 ) (
        unsigned short ConNr
    );

int PRODAVEAPI SetActiveConnection_ex6 (unsigned short ConNr);
typedef int ( PRODAVEAPI *pfn_SetActiveConnection_ex6 ) (
        unsigned short ConNr
    );

int PRODAVEAPI SetPassword_ex6 (unsigned short ConNr, unsigned char *Password);
typedef int ( PRODAVEAPI *pfn_SetPassword_ex6 ) (
		unsigned short ConNr,
        unsigned char *Password
    );

int PRODAVEAPI UnSetPassword_ex6 (unsigned short ConNr);
typedef int ( PRODAVEAPI *pfn_UnSetPassword_ex6 ) (
		unsigned short ConNr
    );

int PRODAVEAPI as_info_ex6 (unsigned long BufLen, AS_INFO_TYPE * pInfoBuffer, unsigned long * pDatLen);
typedef int ( PRODAVEAPI *pfn_as_info_ex6 ) (
        unsigned long BufLen,
        AS_INFO_TYPE * pInfoBuffer,
        unsigned long * pDatLen
    );

int PRODAVEAPI as_zustand_ex6 (unsigned char * pState);
typedef int ( PRODAVEAPI *pfn_as_zustand_ex6 ) (
             unsigned char * pState
    );

int PRODAVEAPI db_buch_ex6 (unsigned long BufLen, unsigned short * pBuchBuffer, unsigned long * pDatLen);
typedef int ( PRODAVEAPI *pfn_db_buch_ex6 ) (
        unsigned long BufLen,
        unsigned short * pBuchBuffer,
        unsigned long * pDatLen
    );

int PRODAVEAPI db_read_ex6 (unsigned short BlkNr, unsigned char DatType, unsigned short StartNr, unsigned long * Amount, unsigned long BufLen, unsigned char * pReadBuffer, unsigned long * pDatLen);
typedef int ( PRODAVEAPI *pfn_db_read_ex6 ) (
        unsigned short BlkNr,
        unsigned char DatType,
        unsigned short StartNr,
        unsigned long * Amount, 
        unsigned long BufLen, 
        unsigned char * pReadBuffer, 
        unsigned long * pDatLen
    );

int PRODAVEAPI db_write_ex6 (unsigned short BlkNr, unsigned char DatType, unsigned short StartNr, unsigned long * Amount, unsigned long BufLen, unsigned char * pWriteBuffer);
typedef int ( PRODAVEAPI *pfn_db_write_ex6 ) (
        unsigned short BlkNr, 
        unsigned char DatType, 
        unsigned short StartNr, 
        unsigned long * Amount, 
        unsigned long BufLen, 
        unsigned char * pWriteBuffer
    );

int PRODAVEAPI bst_read_diag_ex6 (int BlkType, unsigned short StartNr, unsigned long * Amount, unsigned long BufLen, BST_DIAG_TYPE * pDiagBuffer, unsigned long * pDatLen);
typedef int ( PRODAVEAPI *pfn_bst_read_diag_ex6 ) (
        int BlkType, 
        unsigned short StartNr, 
        unsigned long * Amount, 
        unsigned long BufLen, 
        BST_DIAG_TYPE * pDiagBuffer, 
        unsigned long * pDatLen
    );

int PRODAVEAPI bst_read_stat_ex6 (int BlkType, unsigned long BufLen, BST_STAT_TYPE * pStatBuffer, unsigned long * pDatLen);
typedef int ( PRODAVEAPI *pfn_bst_read_stat_ex6 ) (
        int BlkType, 
        unsigned long BufLen, 
        BST_STAT_TYPE * pStatBuffer, 
        unsigned long * pDatLen
    );

int PRODAVEAPI bst_read_ex6 (int BlkType, unsigned short blkNr, unsigned long BufLen, unsigned char * pReadBuffer, unsigned long * pDatLen);
typedef int ( PRODAVEAPI *pfn_bst_read_ex6 ) (
        int BlkType, 
        unsigned short blkNr, 
        unsigned long BufLen, 
        unsigned char * pReadBuffer, 
        unsigned long * pDatLen
    );

int PRODAVEAPI read_diag_buf_ex6 (unsigned long BufLen, unsigned char * pDiagBuffer, unsigned long * pDatLen);
typedef int ( PRODAVEAPI *pfn_read_diag_buf_ex6 ) (
        unsigned long BufLen, 
        unsigned char * pDiagBuffer, 
        unsigned long * pDatLen
    );

int PRODAVEAPI field_read_ex6 (char FieldType, unsigned short BlkNr, unsigned short StartNr, unsigned long Amount, unsigned long BufLen, unsigned char * pBuffer, unsigned long * pDatLen);
typedef int ( PRODAVEAPI *pfn_field_read_ex6 ) (
        char FieldType, 
        unsigned short BlkNr, 
        unsigned short StartNr, 
        unsigned long Amount, 
        unsigned long BufLen, 
        unsigned char * pBuffer, 
        unsigned long * pDatLen
    );
	
int PRODAVEAPI field_write_ex6 (char FieldType, unsigned short BlkNr, unsigned short StartNr, unsigned long Amount, unsigned long BufLen, unsigned char * pBuffer);
typedef int ( PRODAVEAPI *pfn_field_write_ex6 ) (
        char FieldType,
        unsigned short BlkNr, 
        unsigned short StartNr, 
        unsigned long Amount, 
        unsigned long BufLen, 
        unsigned char * pBuffer
    );

int PRODAVEAPI szl_read_ex6 (unsigned short SzlId, unsigned short SzlIndex, unsigned long BufLen, unsigned char * pBuffer, unsigned long * pDatLen);
typedef int ( PRODAVEAPI *pfn_szl_read_ex6 ) ( 
        unsigned short SzlId, 
        unsigned short SzlIndex, 
        unsigned long BufLen, 
        unsigned char * pBuffer, 
        unsigned long * pDatLen
    );
	
int PRODAVEAPI get_pb_lifelist_ex6 (unsigned long BufLen, unsigned char * pBuffer, unsigned long * pDatLen);
typedef int ( PRODAVEAPI *pfn_get_pb_lifelist_ex6 ) ( 
        unsigned long BufLen, 
        unsigned char * pBuffer, 
        unsigned long * pDatLen
    );

int PRODAVEAPI get_ie_lifelist_ex6 (unsigned long BufLen, unsigned char * pBuffer, unsigned long * pDatLen);
typedef int ( PRODAVEAPI *pfn_get_ie_lifelist_ex6 ) ( 
        unsigned long BufLen, 
        unsigned char * pBuffer, 
        unsigned long * pDatLen
    );

int PRODAVEAPI defrag_ex6 ();
typedef int ( PRODAVEAPI *pfn_defrag_ex6 ) ();


int PRODAVEAPI mb_setbit_ex6 (unsigned short MbNr, unsigned short BitNr, unsigned char Value);
typedef int ( PRODAVEAPI *pfn_mb_setbit_ex6 ) (
        unsigned short MbNr, 
        unsigned short BitNr, 
        unsigned char Value
    );

int PRODAVEAPI mb_bittest_ex6 (unsigned short MbNr, unsigned short BitNr, BOOL * pValue);
typedef int ( PRODAVEAPI *pfn_mb_bittest_ex6 ) (
        unsigned short MbNr,
        unsigned short BitNr,
        BOOL * pValue    
    );

int PRODAVEAPI as200_as_info_ex6 (unsigned long BufLen, AS200_INFO_TYPE * pBuffer, unsigned long * pDatLen);
typedef int ( PRODAVEAPI *pfn_as200_as_info_ex6 ) (
        unsigned long BufLen, 
        AS_INFO_TYPE * pBuffer, 
        unsigned long * pDatLen
    );

int PRODAVEAPI as200_as_zustand_ex6 (unsigned char * pState);
typedef int ( PRODAVEAPI *pfn_as200_as_zustand_ex6 ) (
        unsigned char * pState
    );

int PRODAVEAPI as200_field_read_ex6 (char FieldType, unsigned short BlkNr, unsigned short StartNr, unsigned long Amount, unsigned long BufLen, unsigned char * pBuffer, unsigned long * pDatLen);
typedef int ( PRODAVEAPI *pfn_as200_field_read_ex6 ) (
        char FieldType, 
        unsigned short BlkNr, 
        unsigned short StartNr, 
        unsigned long Amount, 
        unsigned long BufLen, 
        unsigned char * pBuffer, 
        unsigned long * pDatLen
    );

int PRODAVEAPI as200_field_write_ex6(char FieldType, unsigned short BlkNr, unsigned short StartNr, unsigned long Amount, unsigned long BufLen, unsigned char * pBuffer);
typedef int ( PRODAVEAPI *pfn_as200_field_write_ex6 ) (
        char FieldType, 
        unsigned short BlkNr, 
        unsigned short StartNr, 
        unsigned long Amount, 
        unsigned long BufLen, 
        unsigned char * pBuffer
    );

int PRODAVEAPI as200_mb_setbit_ex6 (unsigned short MbNr, unsigned short BitNr, unsigned char Value);
typedef int ( PRODAVEAPI *pfn_as200_mb_setbit_ex6 ) (
        unsigned short MbNr, 
        unsigned short BitNr, 
        unsigned char Value
    );

int PRODAVEAPI as200_mb_bittest_ex6 (unsigned short MbNr, unsigned short BitNr, BOOL * pValue);
typedef int ( PRODAVEAPI *pfn_as200_mb_bittest_ex6 ) (
        unsigned short MbNr,
        unsigned short BitNr,
        BOOL * pValue
    );


//int PRODAVEAPI GetErrorMessage_ex6 (int ErrorNr, unsigned long BufLen, unsigned char * pBuffer);
int PRODAVEAPI GetErrorMessage_ex6 (int ErrorNr, ULONG BufLen, char * pBuffer);
typedef int ( PRODAVEAPI *pfn_GetErrorMessage_ex6 ) (
        int ErrorNr,
		ULONG BufLen,
        char * pBuffer
    );


int PRODAVEAPI kg_2_float_ex6 (ULONG kg, float * pieee);
typedef int ( PRODAVEAPI *pfn_kg_2_float_ex6 ) (
		unsigned long kg,
		float * pieee
    );


int PRODAVEAPI float_2_kg_ex6 (float ieee, ULONG * pkg);
typedef int ( PRODAVEAPI *pfn_float_2_kg_ex6 ) (
		float ieee,
		unsigned long * pkg
    );


int PRODAVEAPI gp_2_float_ex6 (ULONG gp, float * pieee);
typedef int ( PRODAVEAPI *pfn_gp_2_float_ex6 ) (
		unsigned long gp,
		float * pieee
    );


int PRODAVEAPI float_2_gp_ex6 (float ieee, ULONG * pgp);
typedef int ( PRODAVEAPI *pfn_float_2_gp_ex6 ) (
		float ieee,
		unsigned long * pgp
    );


BOOL PRODAVEAPI testbit_ex6 (unsigned char Value, USHORT BitNr);
typedef BOOL ( PRODAVEAPI *pfn_testbit_ex6 ) (
		unsigned char Value,
		USHORT BitNr
    );


void PRODAVEAPI byte_2_bool_ex6 (unsigned char Value, BOOL * pBuffer);
typedef void ( PRODAVEAPI *pfn_byte_2_bool_ex6 ) (
		unsigned char Value,
		BOOL * pBuffer
    );


unsigned char PRODAVEAPI bool_2_byte_ex6 (BOOL * pBuffer);
typedef unsigned char ( PRODAVEAPI *pfn_bool_2_byte_ex6 ) (
		BOOL pBuffer
    );


unsigned short PRODAVEAPI kf_2_integer_ex6 (USHORT wValue);
typedef unsigned short ( PRODAVEAPI *pfn_kf_2_integer_ex6 ) (
		unsigned short wValue
    );


unsigned long PRODAVEAPI kf_2_long_ex6 (ULONG dwValue);
typedef void ( PRODAVEAPI *pfn_kf_2_long_ex6 ) (
        unsigned long dwValue
    );


void PRODAVEAPI swab_buffer_ex6 (unsigned char * pBuffer, ULONG Amount);
typedef void ( PRODAVEAPI *pfn_swab_buffer_ex6 ) (
        unsigned char * pTargetBuffer, 
        unsigned char pSourceBuffer, 
        unsigned long Amount
    );


void PRODAVEAPI copy_buffer_ex6 (unsigned char * pTargetBuffer, unsigned char * pSourceBuffer, ULONG Amount);
typedef void ( PRODAVEAPI *pfn_copy_buffer_ex6 ) (
        unsigned char * pTargetBuffer, 
        unsigned char pSourceBuffer, 
        unsigned long Amount
    );


void PRODAVEAPI ushort_2_bcd_ex6 (USHORT * pwValues, ULONG Amount, BOOL InBytechange, BOOL OutBytechange);
typedef void ( PRODAVEAPI *pfn_ushort_2_bcd_ex6 ) (
        unsigned short * pwValues, 
        unsigned long Amount, 
        BOOL InBytechange,
		BOOL OutBytechange
    );


void PRODAVEAPI ulong_2_bcd_ex6 (ULONG * pdwValues, ULONG Amount, BOOL InBytechange, BOOL OutBytechange);
typedef void ( PRODAVEAPI *pfn_ulong_2_bcd_ex6 ) (
        unsigned long * pdwValues, 
        unsigned long Amount, 
        BOOL InBytechange,
		BOOL OutBytechange
    );


void PRODAVEAPI bcd_2_ushort_ex6 (USHORT * pwValues, ULONG Amount, BOOL InBytechange, BOOL OutBytechange);
typedef void ( PRODAVEAPI *pfn_bcd_2_ushort_ex6 ) (
        unsigned short * pwValues, 
        unsigned long Amount, 
        BOOL InBytechange,
		BOOL OutBytechange
    );


void PRODAVEAPI bcd_2_ulong_ex6 (ULONG * pdwValues, ULONG Amount, BOOL InBytechange, BOOL OutBytechange);
typedef void ( PRODAVEAPI *pfn_bcd_2_ulong_ex6 ) (
        unsigned long * pdwValues, 
        unsigned long Amount, 
        BOOL InBytechange,
		BOOL OutBytechange
    );


int PRODAVEAPI GetLoadedConnections_ex6(ULONG BufLen, BOOL *pBuffer);
typedef int ( PRODAVEAPI *fpn_GetLoadedConnections_ex6 ) (
		ULONG BufLen,
        BOOL *pBuffer
    );

int PRODAVEAPI GetPduSize_ex6(unsigned short ConNr);
typedef int ( PRODAVEAPI *fpn_GetPduSize_ex6 ) (
		unsigned short ConNr
    );

// ---------------------------------------------------------------------------
// teleservice functions
// ---------------------------------------------------------------------------

//int PRODAVEAPI ts_dial_ex6 (char * ModemName, char * StandortName, char * TelNo, char * UserName, char * Password, HANDLE WndHdl, UINT Msg, WPARAM WParam, char * LogDevice);
//typedef int ( PRODAVEAPI *pfn_ts_dial_ex6 ) (
//	char * ModemName,
//	char * StandortName,
//	char * TelNo,
//	char * UserName,
//	char * Password,
//	HANDLE WndHdl,
//	UINT Msg,
//	WPARAM WParam,
//	char * LogDevice);


int PRODAVEAPI ts_hang_up_dial_ex6(void);
typedef int ( PRODAVEAPI *pfn_ts_hang_up_dial_ex6 ) (
	void
    );

//int PRODAVEAPI ts_set_ringindicator_ex6(char * ModemName, int NumberOfRings, HANDLE WndHdl, UINT Msg, WPARAM WParam, char * LogDevice);
//typedef int ( PRODAVEAPI *pfn_ts_set_ringindicator_ex6 ) (
//	char * ModemName,
//	int NumberOfRings,
//	HANDLE WndHdl,
//	UINT Msg,
//	WPARAM WParam,
//	char * LogDevice
//    );


int PRODAVEAPI ts_read_info_ex6(TS_USERID * pEventID,unsigned char * pMPIAdr);
typedef int ( PRODAVEAPI *pfn_ts_read_info_ex6 ) (
	TS_USERID * pEventID,
	unsigned char * pMPIAdr
    );


int PRODAVEAPI ts_hang_up_ring_ex6 (void);
typedef int ( PRODAVEAPI *pfn_ts_hang_up_ring_ex6 ) (
	void
    );


int PRODAVEAPI ts_get_modem_name_ex6 (int ModemId, char * Buffer, int * BufferLen);
typedef int ( PRODAVEAPI *pfn_ts_get_modem_name_ex6 ) (
	int ModemId,
	char * Buffer,
	int * BufferLen
    );






#ifdef __cplusplus  // used with C++
}
#endif
// ---------------------------------------------------------------------------

#pragma pack()  // set default packing

#endif  // #ifndef _PRODAVE6_H_

// !-------------------------------------------------------------------------!
// ! Copyright (C) Siemens AG 2004  All Rights Reserved. Confidential        !
// !-------------------------------------------------------------------------!
