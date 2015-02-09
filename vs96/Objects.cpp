/////////////////////////////////////////////////////////////////
// Данный модуль включается как в систему САПР, так и в программу ESS!!!
//----------------------------------------------------------------------
// Ver.2.0.0    17.03.2006   Russian == English
// Ver.2.0.1    20.03.2006
//              - внесен класс CMapStringToNode (был в SF)
/////////////////////////////////////////////////////////////////

// objects.cpp : implementation of the CObject class
//------------------------------------------------------------
// 1.      CKnAttr     -          A T T R I B U T E
// 2. CMapAttributes   - Map  of  A T T R I B U T I E S
// 3.      CExpres     -          E X P R E S S I O N
// 4. CArrayExpression - Array of E X P R E S S I O N S
// 5.     CProduction  -          P R O D U C T I O N
// 6.      CProgMod    -          P R O G R A M  M O D U L
// 7.  CMapProgModuls  - Map  of  P R O G R A M  M O D U L S
// 8.      CSlot       -          S L O T  O F  F R A M E
// 9.  CArraySlots     - Array of S L O T S  O F  F R A M E
// 10.      CFrame      -          F R A M E
// 11.   CMapFrames    - Map  of  F R A M E
// 12.CArrayProduction - Array of P R O D U C T I O N S
// 13.     CStrategy   -          S T R A T E G Y
// 14. CMapStrategies  - Map  of  S T R A T E G I E S
// 15. CDoubleArray    - Array of double value
// 16. CMapValFrame   -  Map of values attributes for Frame
// 17. CBStack        -  Stack with BYTES
// 18. CStrStack      -  Stack with strategies names
// 19. CMapStringToNode  - Map of strings for nodes
// 20  CMenuESS       -  menu position in ESS
// 21. CArrayMenuESS  -  Array of MenuESS positions
//---------------------------------------------------------------------WriteRead
//BOOL ReadDoc(CString FileName, CMapAttributes* pA, CMapProgModuls* pM,
//             CMapStrategies* pS, CMapFrames* pF, CWinApp *pApp)
//------------------------------------------------------------

#include <string.h>
#include "stdafx.h"
#include "..\com_lib\objects.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif
int compare( const void *arg1, const void *arg2 );

int compare( const void *arg1, const void *arg2 )
{  return strcmp( (char*) arg1,  (char*) arg2 ); }

void  PrintMap(CMapStringToString *m_pMapValFrame, CString a);

///////////////////////////////////////////////////////////// CKnAttr
//
// 1.      CKnAttr     - A T T R I B U T E
//
/////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CKnAttr,CObject,VERSIONABLE_SCHEMA|1)
CKnAttr::CKnAttr() { }
CKnAttr::~CKnAttr() { }
//-----------------------------------------------------------
void CKnAttr::Serialize(CArchive& ar)
{
  if (ar.IsStoring()) {
    ar<<m_ShortName; ar<<m_FullName;  ar<<m_Type; ar<<m_Active; ar<<m_Dyn;
    if (m_Type>1)  ar<<m_StrValue;
    else           ar<<m_Value;
  }
  else {
    ar>>m_ShortName; ar>>m_FullName;  ar>>m_Type;  ar>>m_Active; ar>>m_Dyn;
    if (m_Type>1) { ar>>m_StrValue;  m_Value = 0; }
    else          { ar>>m_Value; m_StrValue = ""; }
  }
}

///////////////////////////////////////////////////////////// CMapAttributes
//
// 2. CMapAttributes   - Map of A T T R I B U T I E S
//
/////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMapAttributes, CMapStringToOb)
CMapAttributes::CMapAttributes() { }
//-----------------------------------------------------------
void CMapAttributes::RemoveAll()
{
  POSITION pos;
  CString key;
  CKnAttr *pKnAttr;
  for (pos = GetStartPosition(); pos!=NULL; )  {
    GetNextAssoc(pos, key, pKnAttr); delete pKnAttr;
  }
  CMapStringToOb::RemoveAll();
}

//-----------------------------------------------------------
void CMapAttributes::GetNextAssoc(POSITION& rNextPosition, CString& rKey, CKnAttr*& rValue)
{ CMapStringToOb::GetNextAssoc(rNextPosition, rKey, (CObject *&)rValue); }

//-----------------------------------------------------------
BOOL CMapAttributes::Lookup(const char* key, CKnAttr*& rValue)
{ return(CMapStringToOb::Lookup( key, (CObject *&)rValue)); }

//-----------------------------------------------------------
void CMapAttributes::SetAt(const char* key, CKnAttr* newValue)
{ CMapStringToOb::SetAt(key, (CObject *)newValue); }

//------------------------------------   13.04.98 Красильникова
void CMapAttributes::PrintMapAttribs(CString FileName)
{
  POSITION pos;
  CString key, s1;
  CKnAttr *pKnAttr;
  char buf[30];

  TRY
  {
    _unlink(FileName);
    CFile f(FileName, CFile::modeCreate | CFile::modeWrite);
    for (pos = GetStartPosition(); pos!=NULL; )  {
      GetNextAssoc(pos, s1, pKnAttr);
#ifndef _MSVC9
      sprintf(buf,"%10.2lf",pKnAttr->m_Value);
#else
      sprintf_s(buf,30, "%10.2lf",pKnAttr->m_Value);
#endif

      s1 += (" -> " + pKnAttr->m_StrValue + " " + buf + "\r\n");
      f.Write(s1.GetBuffer(100), s1.GetLength());
    }
  }
  CATCH(CFileException,e)
  {
#ifdef WEB
    printf("Error at the record of file\n");
#else
#ifdef ENG
    AfxMessageBox("Error at the record of file ");
#else
    AfxMessageBox("Ошибка при записи файла ");
#endif
#endif
    return;
  }
  END_CATCH
}

//-----------------------------------------------------------
void CMapAttributes::PrintFullAttribs(CString FileName)
{
  POSITION pos;
  CString key, s1;
  CKnAttr *pKnAttr;
  char Buf[256][256];
  int Count=0;
  TRY
  {
    _unlink(FileName);
    CFile f(FileName, CFile::modeCreate | CFile::modeWrite);
    for (pos = GetStartPosition(); pos!=NULL; )  {
      GetNextAssoc(pos, s1, pKnAttr);
      if (pKnAttr->m_Type == 0) 
        s1 +="  (logical)\n   "; 
      else if (pKnAttr->m_Type == 1) 
        s1 +="  (numeric)\n   "; 
      else
        s1 +="  (string)\n   "; 
      s1 += pKnAttr->m_FullName + "\n\n";
#ifndef _MSVC9
      strcpy(Buf[Count++],(const char*)s1);
#else
      strcpy_s(Buf[Count++],256,(const char*)s1);
#endif
    }
    qsort(Buf,Count,256,compare);
    for (int i=0; i<Count; i++)
      f.Write(Buf[i], strlen(Buf[i]));
  }
  CATCH(CFileException,e)
  {
#ifdef WEB
    printf("Error at the record of file\n");
#else
#ifdef ENG
    AfxMessageBox("Error at the record of file ");
#else
    AfxMessageBox("Ошибка при записи файла ");
#endif
#endif
    return;
  }
  END_CATCH
}

///////////////////////////////////////////////////////////// CExpres
//
// 3.      CExpres     - E X P R E S S I O N
//
/////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CExpres,CObject,0)
CExpres::CExpres() { }
CExpres::~CExpres() { }
//-----------------------------------------------------------
void CExpres::Serialize(CArchive& ar)
{
  if (ar.IsStoring()) { ar<<m_Type; ar<<m_Formula; ar<<m_Value; }
  else                { ar>>m_Type; ar>>m_Formula; ar>>m_Value; }
}

///////////////////////////////////////////////////////////// CArrayExpression
//
// 4. CArrayExpression - Array of E X P R E S S I O N
//
/////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CArrayExpression, CObArray)
CArrayExpression::CArrayExpression() { }
//-----------------------------------------------------------
CExpres* CArrayExpression::GetAt(int nIndex)  const
{ return (CExpres*) CObArray::GetAt(nIndex); }
//-----------------------------------------------------------
void CArrayExpression::SetAt(int nIndex, CExpres* newElement)
{ CObArray::SetAt(nIndex, (CObject *) newElement); }
//-----------------------------------------------------------
int CArrayExpression::Add(CExpres* newElement)
{ return CObArray::Add((CObject *) newElement); }
//-----------------------------------------------------------
void CArrayExpression::RemoveAll()
{ 
  int i;
  CExpres* pExpres;
  for (i=0; i<GetSize(); i++) { pExpres = GetAt(i); delete pExpres; }
  CObArray::RemoveAll();
}

///////////////////////////////////////////////////////////// CProduction
//
// 5.      CProduction - P R O D U C T I O N
//
/////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CProduction,CObject,0)
//-----------------------------------------------------------
CProduction::CProduction() { }
CProduction::~CProduction() { m_ArrayExpression.RemoveAll(); }
//-----------------------------------------------------------
void CProduction::Serialize(CArchive& ar)
{
  int i,k;
  CExpres * pExpres;

  if (ar.IsStoring()) {
    k = m_ArrayExpression.GetSize();
    ar<<(WORD)k;
    for (i=0; i<k; i++) {
      pExpres = m_ArrayExpression.GetAt(i);
      pExpres->Serialize(ar);
    }
   ar<<m_Formula; ar<<m_Type; ar<<m_NameCon;
  }
  else {
    WORD n;
    ar>>n; k = (int)n;
    for (i=0; i<k; i++) {
      pExpres = new CExpres();
      pExpres->Serialize(ar);
      m_ArrayExpression.Add(pExpres);
    }
    ar>>m_Formula; ar>>m_Type; ar>>m_NameCon;
  }
}

//----------------------------------------------------------- CreateCondition
CString CProduction::CreateCondition()
{
  int k,num,p0;
  CString Formula;
  CString W,W1;
  CExpres* pExpres;

  num = m_ArrayExpression.GetSize();
  Formula="";
  W = m_Formula;
  while (!W.IsEmpty()) {
    if (W[0]!= '{') { Formula+=W[0]; W=W.Right(W.GetLength()-1); continue; }
    Formula += "{ "; W=W.Right(W.GetLength()-1);
    p0 = W.Find('}');
    if (p0 <0) return "Error!!!";
    W1 = W.Left(p0);
    W = W.Right(W.GetLength()-p0-1);
    k = atoi((const char*)W1);
    if (k<1 || k>num) return "Error!!!";
    k--;
    pExpres = m_ArrayExpression.GetAt(k);
    Formula +=pExpres->m_Formula + " }";
  }
  return Formula;
}

//-----------------------------------------------------------
void CProduction::operator =( const CProduction* src )
{
  m_ArrayExpression.RemoveAll();
  int i,k;
  CExpres *pExpres;
  CExpres *pExpresOld;
  k = src->m_ArrayExpression.GetSize();
  for (i = 0; i<k; i++) {
    pExpres = new CExpres ();
    pExpresOld = src->m_ArrayExpression.GetAt(i);
    pExpres->m_Formula = pExpresOld->m_Formula;
    pExpres->m_Type = pExpresOld->m_Type;
    pExpres->m_Value = pExpresOld->m_Value;
    m_ArrayExpression.Add(pExpres);
  }
  m_Formula = src->m_Formula;
  m_Type = src->m_Type;
  m_NameCon = src->m_NameCon;
}

///////////////////////////////////////////////////////////// CProgMod
//
// 6.      CProgMod    - P R O G R A M  M O D U L
//
/////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CProgMod,CObject,VERSIONABLE_SCHEMA|1)
CProgMod::CProgMod() { }
CProgMod::~CProgMod() { m_AttrNames.RemoveAll(); }
//-----------------------------------------------------------
void CProgMod::Serialize(CArchive& ar)
{
  int nVersion = 1;
  if (ar.IsStoring()) {
    ar<<m_Name; ar<<m_Comment; ar<<m_Menu; ar<<m_Type;  ar<<(WORD)m_Undo;
  }
  else { 
    WORD n;
    nVersion = ar.GetObjectSchema();
    ar>>m_Name; ar>>m_Comment; ar>>m_Menu; ar>>m_Type; 
    ar>>n; m_Undo = (BOOL)n;  // ar>>m_ParmW; ar>>m_ParmS;
  }
  m_Prod.Serialize(ar);
  m_AttrNames.Serialize(ar);

  if (nVersion==1)
    m_ParamNames.Serialize(ar);
}

///////////////////////////////////////////////////////////// CMapProgModuls
//
// 7.  CMapProgModuls  - Map of P R O G R A M  M O D U L S
//
/////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMapProgModuls, CMapStringToOb)
CMapProgModuls::CMapProgModuls() { }
//-----------------------------------------------------------
void CMapProgModuls::RemoveAll()
{
  POSITION pos;
  CString key;
  CProgMod *pProgMod;
  for (pos = GetStartPosition(); pos!=NULL; )  {
    GetNextAssoc(pos, key, pProgMod); delete pProgMod;
  }
  CMapStringToOb::RemoveAll();
}

//-----------------------------------------------------------
void CMapProgModuls::GetNextAssoc(POSITION& rNextPosition, CString& rKey, CProgMod*& rValue)
{CMapStringToOb::GetNextAssoc(rNextPosition, rKey, (CObject *&)rValue); }

//-----------------------------------------------------------
BOOL CMapProgModuls::Lookup(const char* key, CProgMod*& rValue)
{ return(CMapStringToOb::Lookup( key, (CObject *&)rValue)); }

//-----------------------------------------------------------
void CMapProgModuls::SetAt(const char* key, CProgMod* newValue)
{ CMapStringToOb::SetAt(key, (CObject *)newValue); }

//-----------------------------------------------------------
void CMapProgModuls::PrintFullProgs(CString FileName)
{
  POSITION pos;
  CString key, s1;
  CProgMod *pProgMod;
  char Buf[64][512];
  int Count=0;
///////////////////////////////////////////////////////
//   CString m_Name;         // Name of program modul
//   CString m_Menu;         // String to menu of main program
//   CString m_Comment;      // Comment for program modul
//   BOOL m_Undo;            // Possibility of Undo operation
//   BYTE m_Type;            // 0 - internal Windows, 1 - external Windows,
                            // 2 - DOS  3 - special DOS  4... - reserved
//   CStringArray m_AttrNames;   // Map of modul setting attributes
//   CStringArray m_ParamNames;   // Map of modul setting parameters
//   CProduction m_Prod;     // Condition of execution
  s1 = "------------------------------\r\n";

  TRY
  {
    _unlink(FileName);
    CFile f(FileName, CFile::modeCreate | CFile::modeWrite);
    CString w;
    int j;
    f.Write(s1, strlen(s1));
    for (pos = GetStartPosition(); pos!=NULL; )  {
      GetNextAssoc(pos, s1, pProgMod);
      s1 = pProgMod->m_Menu + "\r\n"+ s1;
      s1 += "\r\n   ";
      w = pProgMod->m_Comment;
      while ((j=w.FindOneOf("\r\n"))>=0) w = w.Left(j) + w.Mid(j+1);
      s1 += w + "\r\n   ";
      w = pProgMod->m_Prod.CreateCondition();
      if (!w.IsEmpty())
#ifdef ENG
        s1 += w + "\t\t- start condition\r\n   ";
      if (!pProgMod->m_Undo) s1 += "Backoff is impossible!\r\n   ";
#else
        s1 += w + "\t\t- условие запуска\r\n   ";
      if (!pProgMod->m_Undo) s1 += "Нет возможности отката!\r\n   ";
#endif

      if (pProgMod->m_ParamNames.GetSize()>0) 
      {
#ifndef ENG
        s1 += "Параметры запуска: ";
#else
        s1 += "Start parameters: ";
#endif
        for (int yy=0; yy < pProgMod->m_ParamNames.GetSize(); yy++)
        {
          s1 += pProgMod->m_ParamNames[yy] + "  ";
        }
        s1 += "\r\n   ";
      }

      if (pProgMod->m_AttrNames.GetSize()>0) 
      {
#ifndef ENG
        s1 += "Устанавливаемые атрибуты: ";
#else
        s1 += "Established attributes: ";
#endif
        for (int yy=0; yy < pProgMod->m_AttrNames.GetSize(); yy++)
        {
          s1 += pProgMod->m_AttrNames[yy] + "  ";
        }
        s1 += "\r\n   ";
      }
#ifndef ENG
      s1 += "Тип модуля: ";
#else
      s1 += "Type of modul: ";
#endif
      switch(pProgMod->m_Type)
      {
#ifdef ENG
      case 0:s1 += "Windows application"; break; 
      case 1:s1 += "DOS application"; break;
      case 2:s1 += "Special DOS program"; break;
      case 3:s1 += "Internal for Windows"; break;
#else
      case 0:s1 += "приложение Windows"; break; 
      case 1:s1 += "приложение DOS"; break;
      case 2:s1 += "специальный DOS"; break;
      case 3:s1 += "внутренний Windows"; break;
#endif
      }
      s1 += "\r\n\r\n------------------------------\r\n";
#ifndef _MSVC9
      strcpy(Buf[Count++],(const char*)s1);
#else
      strcpy_s(Buf[Count++],512,(const char*)s1);
#endif
    }
    qsort(Buf,Count,512,compare);
    for (int i=0; i<Count; i++)
      f.Write(Buf[i], strlen(Buf[i]));
  }
  CATCH(CFileException,e)
  { 
#ifdef WEB
    printf("Error at the record of file\n");
#else
#ifdef ENG
    AfxMessageBox("Error at the record of file ");
#else
    AfxMessageBox("Ошибка при записи файла ");
#endif
#endif
    return;
  }
  END_CATCH
}

///////////////////////////////////////////////////////////// CSlot
//
// 8.      CSlot       - S L O T  O F  F R A M E
//
/////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CSlot,CObject,0)
CSlot::CSlot() { }
CSlot::~CSlot() { m_AttrNames.RemoveAll(); }
//-----------------------------------------------------------
void CSlot::Serialize(CArchive& ar)
{
  m_AttrNames.Serialize(ar);
  if (ar.IsStoring())  ar<<m_Name;   else  ar>>m_Name;
}

///////////////////////////////////////////////////////////// CArraySlots
//
// 9.  CArraySlots     - Array of S L O T S  O F  F R A M E
//
/////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CArraySlots, CObArray)
CArraySlots::CArraySlots() { }
//-----------------------------------------------------------
CSlot* CArraySlots::GetAt(int nIndex)  const
{ return (CSlot*) CObArray::GetAt(nIndex); }
//-----------------------------------------------------------
void CArraySlots::SetAt(int nIndex, CSlot* newElement)
{ CObArray::SetAt(nIndex, (CObject *) newElement); }
//-----------------------------------------------------------
int CArraySlots::Add(CSlot* newElement)
{ return CObArray::Add((CObject *) newElement); }
//-----------------------------------------------------------
CSlot* CArraySlots::GetForName(CString Name)
{
  int i;
  CSlot* pSlot;
  for (i=0; i<GetSize(); i++)
  { pSlot = GetAt(i); if (pSlot->m_Name == Name) return pSlot;  }
  return NULL;
}

//-----------------------------------------------------------
void CArraySlots::RemoveAll()
{
  CSlot* pSlot;
  for (int i=0; i<GetSize(); i++) { pSlot = GetAt(i); delete pSlot; }
  CObArray::RemoveAll();
}

///////////////////////////////////////////////////////////// CFrame
//
// 10.      CFrame      - F R A M E
//
/////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CFrame,CObject,0)
CFrame::CFrame() { }
CFrame::~CFrame() { m_ArraySlots.RemoveAll(); }
//-----------------------------------------------------------
void CFrame::Serialize(CArchive& ar)
{
  if (ar.IsStoring()) { ar<<m_Name; ar<<m_Comment; ar<<m_Specimen; }
  else                { ar>>m_Name; ar>>m_Comment; ar>>m_Specimen; }
  m_ArraySlots.Serialize(ar);
}

///////////////////////////////////////////////////////////// CMapFrames
//
// 11.   CMapFrames    - Map  of  F R A M E
//
/////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMapFrames, CMapStringToOb)
CMapFrames::CMapFrames() { }
//-----------------------------------------------------------
void CMapFrames::RemoveAll()
{
  POSITION pos;
  CString key;
  CFrame *pFrame;
  for (pos = GetStartPosition(); pos!=NULL; )  {
    GetNextAssoc(pos, key, pFrame);  delete pFrame;
  }
  CMapStringToOb::RemoveAll();
}
//------------------------------------------------------------
CFrame* CMapFrames::GetFrame(int Num)
{
  POSITION pos;
  CString key;
  CFrame *pFrame;
  int n=0;
  for (pos = GetStartPosition(); pos!=NULL; n++)  {
    GetNextAssoc(pos, key, pFrame); if (Num == n) return pFrame;
  }
  return NULL;
}
//---------------------------------------------------------------
void CMapFrames::GetNextAssoc(POSITION& rNextPosition, CString& rKey, CFrame*& rValue)
{CMapStringToOb::GetNextAssoc(rNextPosition, rKey, (CObject *&)rValue); }
//---------------------------------------------------------------
BOOL CMapFrames::Lookup(const char* key, CFrame*& rValue)
{ return(CMapStringToOb::Lookup( key, (CObject *&)rValue)); }
//---------------------------------------------------------------
void CMapFrames::SetAt(const char* key, CFrame* newValue)
{ CMapStringToOb::SetAt(key, (CObject *)newValue); }

///////////////////////////////////////////////////////////// CArrayProduction
//
// 12.CArrayProduction - Array of P R O D U C T I O N S
//
/////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CArrayProduction, CObArray)
CArrayProduction::CArrayProduction() { }
//-----------------------------------------------------------
CProduction* CArrayProduction::GetAt(int nIndex)  const
{ return (CProduction*) CObArray::GetAt(nIndex); }
//-----------------------------------------------------------
void CArrayProduction::SetAt(int nIndex, CProduction* newElement)
{ CObArray::SetAt(nIndex, (CObject *) newElement); }
//-----------------------------------------------------------
int CArrayProduction::Add(CProduction* newElement)
{ return CObArray::Add((CObject *) newElement); }
//-----------------------------------------------------------
void CArrayProduction::RemoveAll()
{
  int i;
  CProduction* pProd;
  for (i=0; i<GetSize(); i++) { pProd = GetAt(i);  delete pProd; }
  CObArray::RemoveAll();
}

///////////////////////////////////////////////////////////// CStrategy
//
// 13.     CStrategy   - S T R A T E G Y
//
/////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CStrategy,CObject,0)
CStrategy::CStrategy() { }
CStrategy::~CStrategy() {m_ArrayProduction.RemoveAll(); }
//-----------------------------------------------------------
void CStrategy::Serialize(CArchive& ar)
{
  if (ar.IsStoring()) { ar<<m_Name; ar<<m_Comment; }
  else                { ar>>m_Name; ar>>m_Comment; }
  m_ArrayProduction.Serialize(ar);
}

/////////////////////////////////////////////////////////////
//
// 14.  CMapStrategies  - Map  of  S T R A T E G I E S
//
/////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMapStrategies, CMapStringToOb)
CMapStrategies::CMapStrategies() { }
//-----------------------------------------------------------
void CMapStrategies::RemoveAll()
{
  POSITION pos;
  CString key;
  CStrategy *pS;
  for (pos=GetStartPosition(); pos!=NULL; ) { GetNextAssoc(pos,key,pS); delete pS; }
  CMapStringToOb::RemoveAll();
}
//------------------------------------------------------------
CStrategy* CMapStrategies::GetStrategy(int Num)
{ 
  POSITION pos;
  CString key;
  CStrategy *pStrategy;
  int n=0;
  for (pos = GetStartPosition(); pos!=NULL; n++)  {
    GetNextAssoc(pos, key, pStrategy); if (Num == n) return pStrategy;
  }
  return NULL;
}
//---------------------------------------------------------------
void CMapStrategies::GetNextAssoc(POSITION& rNextPos, CString& rKey, CStrategy*& rValue)
{CMapStringToOb::GetNextAssoc(rNextPos, rKey, (CObject *&)rValue); }
//---------------------------------------------------------------
BOOL CMapStrategies::Lookup(const char* key, CStrategy*& rValue)
{ return(CMapStringToOb::Lookup( key, (CObject *&)rValue)); }
//---------------------------------------------------------------
void CMapStrategies::SetAt(const char* key, CStrategy* newValue)
{ CMapStringToOb::SetAt(key, (CObject *)newValue); }

///////////////////////////////////////////////////////////// CArrayExpression
//
// 15. CDoubleArray    - Array of double value
//
/////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CDoubleArray, CObArray)
CDoubleArray::CDoubleArray() { }
CDoubleArray::~CDoubleArray() {RemoveAll(); }
//-----------------------------------------------------------
double* CDoubleArray::GetAt(int nIndex)  const
{ return (double*) CObArray::GetAt(nIndex); }
//-----------------------------------------------------------
void CDoubleArray::SetAt(int nIndex, double newElement)
{ 
  double* dd,*pD; dd = new double;  *dd = newElement;
  if (nIndex<GetSize()) {
    pD = GetAt(nIndex); delete pD;
  }
  CObArray::SetAt(nIndex, (CObject *) dd); 
}
//-----------------------------------------------------------
void CDoubleArray::SetAtGrow(int nIndex, double newElement)
{
  double* dd,*pD; dd = new double;  *dd = newElement;
  if (nIndex<GetSize()) {
    pD = GetAt(nIndex); delete pD;
  }
  CObArray::SetAtGrow(nIndex, (CObject *) dd); 
}
//-----------------------------------------------------------
int CDoubleArray::Add(double newElement)
{
  double* dd; dd = new double;  *dd = newElement;
  return CObArray::Add((CObject *) dd); 
}
//-----------------------------------------------------------
void CDoubleArray::RemoveAll()
{
  int i;
  double* pD;
  for (i=0; i<GetSize(); i++) { pD = GetAt(i); delete pD; }
  CObArray::RemoveAll();
}

///////////////////////////////////////////////////////////// CMapValFrame
//
// 16. CMapValFrame - Map of values attributes for Frame
//
/////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMapValFrame, CMapStringToString)
//  CMapAttributes* m_pMapAttributes;
//-----------------------------------------------------------
BOOL CMapValFrame::GetAt(CString key, CString& res)
{
  return  Lookup(key, res);
}
//-----------------------------------------------------------
BOOL CMapValFrame::GetAt(CString key, double& d)
{
  CString res;
  d=0;
  if ( Lookup(key, res)== FALSE) return FALSE;
  d = atof((const char *)res);
  return TRUE;
}
//-----------------------------------------------------------
void CMapValFrame::SetAt(const char* key, double newValue)
{
  char buf[100];
  CString w;
#ifndef _MSVC9
  sprintf(buf, "%.2lf", newValue);
#else
  sprintf_s(buf,100, "%.2lf", newValue);
#endif
  w = buf;
  CMapStringToString::SetAt(key, w);
}
//-----------------------------------------------------------
void CMapValFrame::SetAt(const char* key, CString newValue)
{ CMapStringToString::SetAt(key, newValue); }
//-----------------------------------------------------------
BOOL  CMapValFrame::WriteToIni(CString sFile, CString Section)
{
  POSITION pos;
  CString key, value;
  for (pos = GetStartPosition(); pos; )  {
    GetNextAssoc(pos, key, value);
    if ( WritePrivateProfileString(Section, key, value, sFile) == FALSE)  return FALSE;
  }
  return TRUE;
}
//-----------------------------------------------------------
BOOL  CMapValFrame::ReadFromIni(CString sFile, CString Section, CStringArray &whatAttr)
{
  CString key, value;
  char Buf[5000], Buf1[50];
  int i, j;
  CStringArray work;

  for (i=0; i< whatAttr.GetSize(); i++)
    work.Add(whatAttr.GetAt(i)); 

  if (work.GetSize()== 0 )  return TRUE;
  if ( GetPrivateProfileString(Section, NULL, "", Buf, 5000, sFile) == FALSE)
  { return FALSE; }
  i=0;
  for (int num = 0;(int)strlen(Buf+i)>0 && num < work.GetSize(); num++)  {
    key = Buf+i;
    GetPrivateProfileString(Section, key, "",Buf1, 5000, sFile);
    value = Buf1;
    SetAt(key, value);
    for (j=0; j<work.GetSize(); j++)  {
      if (key == work.GetAt(j))  {
        work.SetAt(j, "");
        break;
      }
    }
    if (j == work.GetSize())
    {
#ifdef WEB
      printf("Procedure has set a attribute %s\nwhich is not required\n", key);
#else
#ifdef ENG
      AfxMessageBox("Procedure has set a attribute " + key +
        " which is not required");
#else
      AfxMessageBox("Проектная операция установила атрибут " + key +
                    ", который не требуется");
#endif
#endif
    }
    i += (int)strlen(key)+1;
  }
  for (j=0; j<work.GetSize(); j++)
    if (work.GetAt(j) != "")
    {
#ifdef WEB
      printf("Procedure hasn't set the attribute %s\n", work.GetAt(j));
#else
#ifdef ENG
      AfxMessageBox("Procedure hasn't set the attribute " + 
#else
      AfxMessageBox("Проектная операция не установила атрибут " + 
#endif
        work.GetAt(j) );
#endif
    }
  CMapStringToString  *pSt = (CMapStringToString  *)this;
  return TRUE;
}

//-----------------------------------------------------------
int CMapValFrame::GetAt(CMapAttributes* m_pMapAttributes, CString key,
                        CString& res, double& d)
{
  CKnAttr *pKnAttr;
  if (m_pMapAttributes->Lookup(key, pKnAttr) == FALSE)  return -1;
  if ( Lookup(key, res)== FALSE) return -1;
  if (pKnAttr->m_Type == 2)  return 2;
  d = atof((const char *)res);
  return pKnAttr->m_Type;
}

//--------------------------------------------- 29.04.1998 Красильникова
BOOL CMapValFrame::CheckAttr(const char* key, CString Value)
{
  CString sRes;
  BOOL bRes =  Lookup(key, sRes);
  if(!bRes)  return FALSE;
  return( sRes == Value);
}

//--------------------------------------------- 29.04.1998 Красильникова
BOOL CMapValFrame::CheckAttr(const char* key, double Value)
{
  CString sRes;
  double d;
  BOOL bRes =  Lookup(key, sRes);
  if(!bRes)  return FALSE;
  d = atof((const char *)sRes);
  return( d == Value);
}

///////////////////////////////////////////////////////////// CBStack
//
// 17. CBStack    -  stack with BYTES
//
/////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CBStack, CByteArray)
CBStack::CBStack() { }
//---------------------------------------------------------------

BOOL CBStack::Pop(BYTE& El)
{
  int i;
  if ((i=GetUpperBound())<0) return FALSE;
  El = CByteArray::GetAt(i);
  CByteArray::RemoveAt(i);
  return TRUE;
}

//---------------------------------------------------------------
void CBStack::Push(BYTE El) { CByteArray::Add(El); }
//---------------------------------------------------------------
void CBStack::Clear() { BYTE El;  while(Pop(El));  }

///////////////////////////////////////////////////////////// CStrStack
//
// 18. CStrStack    -  Stack with strategies names
//
/////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CStrStack, CStringArray)
CStrStack::CStrStack() { }
//---------------------------------------------------------------

BOOL CStrStack::Pop(CString& El)
{
  int i;
  if ((i=GetUpperBound())<0) return FALSE;
  El = CStringArray::GetAt(i);
  CStringArray::RemoveAt(i);
  return TRUE;
}

//---------------------------------------------------------------
void CStrStack::Push(CString El) { CStringArray::Add(El); }
//---------------------------------------------------------------
void CStrStack::Clear() { CString El;  while(Pop(El)); }


//////////////////////////////////////////////////////  CMapStringToNode
//
// 19. CMapStringToNode  - Map of strings for nodes
//
/////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMapStringToNode, CMapStringToOb)
//--------------------------------------------------------------- Constructor
CMapStringToNode::CMapStringToNode()  { }

//---------------------------------------------------------------
void CMapStringToNode::RemoveAll()
{
  POSITION pos;
  CString key;
  CPNode *pNode;

#ifdef _DEBUG
  AssertValid();
#endif
  pos = GetStartPosition();
  for (; pos!=NULL; )  {
    GetNextAssoc(pos, key ,pNode);
    delete pNode;
  }
  CMapStringToOb::RemoveAll();
}

//---------------------------------------------------------------
void CMapStringToNode::RemoveKey(CString Buf)
{
  CPNode *pNode;

#ifdef _DEBUG
  AssertValid();
#endif
  if (Lookup(Buf, pNode) == FALSE)  {

#ifdef WEB
    printf("119: Program error\n");
#else
#ifdef ENG
    AfxMessageBox("119: Program error");
#else
    AfxMessageBox("119: Программная ошибка");
#endif
#endif
    return;
  }
  delete pNode;
  CMapStringToOb::RemoveKey(Buf);
}

//---------------------------------------------------------------
void CMapStringToNode::GetNextAssoc(POSITION& rNextPosition, CString& rKey,
                     CPNode*& rValue)
{
#ifdef _DEBUG
  AssertValid();
#endif
  CMapStringToOb::GetNextAssoc(rNextPosition, rKey, (CObject *&)rValue);
}

//---------------------------------------------------------------
BOOL CMapStringToNode::Lookup(const char* key, CPNode*& rValue)
{
#ifdef _DEBUG
  AssertValid();
#endif
  return(CMapStringToOb::Lookup( key, (CObject *&)rValue));
}
//---------------------------------------------------------------
void CMapStringToNode::SetAt(const char* key, CPNode* newValue)
{
#ifdef _DEBUG
  AssertValid();
#endif
  CMapStringToOb::SetAt(key, (CObject *)newValue);
}
#ifdef _DEBUG
//--------------------------------------
void CMapStringToNode::AssertValid() const
{
  CMapStringToOb::AssertValid();
}

//------------------------------------------------
void CMapStringToNode::Dump(CDumpContext& dc) const
{
  AssertValid();
  CMapStringToOb::Dump(dc);
}
#endif

///////////////////////////////////////////////////////////// CMenuESS
//
// 20. CMenuESS       - menu position in ESS
//
/////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CMenuESS,CObject,0)
CMenuESS::CMenuESS() { }
CMenuESS::~CMenuESS() { }
//-----------------------------------------------------------
// Attributes

void CMenuESS::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    ar<<m_Name;  ar<<m_Label;  ar<<(int)m_hParent;  ar<<(int)m_hOwn;
  }
  else
  {
    ar>>m_Name;  ar>>m_Label; int v;
    ar>>v; m_hParent = (struct _TREEITEM *)v;
    ar>>v; m_hOwn = (struct _TREEITEM *)v;
  }
}

///////////////////////////////////////////////////////////// CArrayMenuESS
//
// 21.  CArrayMenuESS     - Array of MenuESS positions
//
/////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CArrayMenuESS, CObArray)
CArrayMenuESS::CArrayMenuESS() { }
//-----------------------------------------------------------
CMenuESS* CArrayMenuESS::GetAt(int nIndex)  const
{ return (CMenuESS*) CObArray::GetAt(nIndex); }
//-----------------------------------------------------------
int CArrayMenuESS::Add(CMenuESS* newElement)
{ return CObArray::Add((CObject *) newElement); }
//-----------------------------------------------------------
void CArrayMenuESS::RemoveAll()
{ CMenuESS* pMenuESS;
  for (int i=0; i<GetSize(); i++) { pMenuESS = GetAt(i); delete pMenuESS; }
  CObArray::RemoveAll();
}


//---------------------------------------------------------------------WriteRead
BOOL ReadDoc(CString FileName,
             CMapAttributes* pA, CMapProgModuls* pM,
             CMapStrategies* pS, CMapFrames* pF, CWinApp *pApp)
{
  CFile f;
  int li;
  CFrame *pFr;
  CSlot *pSl;
  CKnAttr *pKnAttr;
  CString ls, lsw1, lsw2;

  if( !f.Open( (const char *)FileName, CFile::modeRead ) ) return FALSE;
  CArchive ar( &f,CArchive::load);
  pA->Serialize(ar);
  pM->Serialize(ar);
  pS->Serialize(ar);
  pF->Serialize(ar);

#ifdef ENG
  lsw1 = "Target";
  lsw2 = "Final target";
#else
  lsw1 = "Цель";
  lsw2 = "Конечная цель";
#endif
  if (pF->Lookup(lsw1, pFr)) {
    for (li= 0; li< pFr->m_ArraySlots.GetSize(); li++)  {
      pSl =  pFr->m_ArraySlots.GetAt(li);
      if (pSl->m_Name != lsw2) continue;
      for (li=0; li< pSl->m_AttrNames.GetSize(); li++)  {
        ls = pSl->m_AttrNames.GetAt(li);
        if (pA->Lookup(ls, pKnAttr))  
        pKnAttr->m_Value = pApp->GetProfileInt("AIM", pKnAttr->m_ShortName, 0);
      }
      break;
    }
  }
  return TRUE;
}
