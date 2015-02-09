// Last modified - 20.03.2006
#ifndef OBJECTS
#define OBJECTS

class CSF;
class CPNode;
//------------------------------------------------------------
// 1.      CKnAttr     -          A T T R I B U T E
// 2. CMapAttributes   - Map  of  A T T R I B U T I E S
// 3.      CExpres     -          E X P R E S S I O N
// 4. CArrayExpression - Array of E X P R E S S I O N S
// 5.      CProgMod    -          P R O G R A M  M O D U L
// 6.  CMapProgModuls  - Map  of  P R O G R A M  M O D U L S
// 7.      CSlot       -          S L O T  O F  F R A M E
// 8.  CArraySlots     - Array of S L O T S  O F  F R A M E
// 9.      CFrame      -          F R A M E
// 10.   CMapFrames    - Map  of  F R A M E
// 11.     CProduction -          P R O D U C T I O N
// 12.CArrayProduction - Array of P R O D U C T I O N S
// 13.     CStrategy   -          S T R A T E G Y
// 14. CMapStrategies  - Map  of  S T R A T E G I E S
// 15. CDoubleArray    - Array of double values
// 16. CMapValFrame - Map of values attributes for Frame
// 17. CBStack    -   Stack with BYTES
// 18. CStrStack     -  Stack with strategies names
// 19. CPNodeArraySmall     -  Special array of nodes CPNode
// 20. CPNode     -  node of SF net
// 21. CPNodeArray     -  Array of nodes CPNode
// 22. CMapStringToNode - Map of strings for nodes
// 23. CMenuESS       - menu position in ESS
// 24. CArrayMenuESS     - Array of MenuESS positions
//------------------------------------------------------------

///////////////////////////////////////////////////////////// CKnAttr
//
// 1.      CKnAttr     - A T T R I B U T E
//
/////////////////////////////////////////////////////////////
class CKnAttr : public CObject
{  DECLARE_SERIAL(CKnAttr)
public:
  CKnAttr(); // Construction
// Attributes
  CString m_FullName;   // For full name of attributes
  CString m_ShortName;  // For short name of attributes
  BYTE m_Type;          // 0 - logical, 1 - integer, 2 - string
  BYTE m_Active;        // 1 - active, 0 - passive
  BYTE m_Dyn;           // 1 - dynamic, 0 - static
  CString m_StrValue;   // For string value
  double m_Value;       // For logical or integer value
  ~CKnAttr();
  void Serialize(CArchive&);
};

///////////////////////////////////////////////////////////// CMapAttributes
//
// 2. CMapAttributes   - Map of A T T R I B U T I E S
//
/////////////////////////////////////////////////////////////
class CMapAttributes : public CMapStringToOb
{
public:
  CMapAttributes();
  DECLARE_DYNCREATE(CMapAttributes)
// Operations
public:
  void RemoveAll();
  void GetNextAssoc(POSITION& rNextPosition, CString& rKey, CKnAttr*& rValue);
  BOOL Lookup(const char* key, CKnAttr*& rValue);
  void SetAt(const char* key, CKnAttr* newValue);
  void PrintMapAttribs(CString FileName);
  void PrintFullAttribs(CString FileName);
};

///////////////////////////////////////////////////////////// CExpres
//
// 3.      CExpres     - E X P R E S S I O N
//
/////////////////////////////////////////////////////////////
class CExpres : public CObject
{  DECLARE_SERIAL(CExpres)
public:
  CExpres();  // Construction
// Attributes
  BYTE m_Type;        // 0 - logical, 1 - integer, 2 - string
  CString m_Formula;  // For string value
  WORD m_Value;       // For  value (TRUE / FALSE)
  ~CExpres();
  void Serialize(CArchive&);
};

///////////////////////////////////////////////////////////// CArrayExpression
//
// 4. CArrayExpression - Array of E X P R E S S I O N
//
/////////////////////////////////////////////////////////////
class CArrayExpression : public CObArray
{
public:
  CArrayExpression();
  DECLARE_DYNCREATE(CArrayExpression)
// Accessing elements
  CExpres* GetAt(int nIndex) const;
  void SetAt(int nIndex, CExpres* newElement);
  int Add(CExpres* newElement);
  void RemoveAll();
};

///////////////////////////////////////////////////////////// CProduction
//
// 11.      CProduction - P R O D U C T I O N
//
/////////////////////////////////////////////////////////////
class CProduction : public CObject
{  DECLARE_SERIAL(CProduction)
public:
  CProduction();  // Construction
// Attributes
  CArrayExpression m_ArrayExpression;     // Left part of production
  CString m_Formula;
  BYTE m_Type;    // Right part of production:  0 - strategy, 1 - modul
  CString m_NameCon;  // Name of strategy or modul
  ~CProduction();
  void Serialize(CArchive&);
  CString CreateCondition();
  void operator =( const CProduction* src );
};

///////////////////////////////////////////////////////////// CProgMod
//
// 5.      CProgMod    - P R O G R A M  M O D U L
//
/////////////////////////////////////////////////////////////
class CProgMod : public CObject
{  DECLARE_SERIAL(CProgMod)
public:
  CProgMod();  // Construction
// Attributes
  CString m_Name;         // Name of program modul
  CString m_Menu;         // String to menu of main program
  CString m_Comment;      // Comment for program modul
  BOOL m_Undo;            // Possibility of Undo operation
  BYTE m_Type;            // 0 - internal Windows, 1 - external Windows,
                          // 2 - DOS  3 - special DOS  4... - reserved
  CStringArray m_AttrNames;   // Map of modul setting attributes
  CStringArray m_ParamNames;  // Map of modul using parameters
  CProduction m_Prod;     // Condition of execution
  ~CProgMod();
  void Serialize(CArchive&);
};

///////////////////////////////////////////////////////////// CMapProgModuls
//
// 6.  CMapProgModuls  - Map of P R O G R A M  M O D U L S
//
/////////////////////////////////////////////////////////////
class CMapProgModuls : public CMapStringToOb
{
public:
  CMapProgModuls();
  DECLARE_DYNCREATE(CMapProgModuls)
// Operations
  void GetNextAssoc(POSITION& rNextPosition, CString& rKey, CProgMod*& rValue);
  BOOL Lookup(const char* key, CProgMod*& rValue);
  void SetAt(const char* key, CProgMod* newValue);
  void RemoveAll();
  void PrintFullProgs(CString FileName);
};

///////////////////////////////////////////////////////////// CSlot
//
// 7.      CSlot       - S L O T  O F  F R A M E
//
/////////////////////////////////////////////////////////////
class CSlot : public CObject
{  DECLARE_SERIAL(CSlot)
public:
  CSlot();  // Construction
// Attributes
  CString m_Name;         // Name of Slot
  CStringArray m_AttrNames;   // Map of slot attributes
  ~CSlot();
  void Serialize(CArchive&);
};

///////////////////////////////////////////////////////////// CArraySlots
//
// 8.  CArraySlots     - Array of S L O T S  O F  F R A M E
//
/////////////////////////////////////////////////////////////
class CArraySlots : public CObArray
{
public:
  CArraySlots();
  DECLARE_DYNCREATE(CArraySlots)
// Accessing elements
  CSlot* GetAt(int nIndex) const;
  void SetAt(int nIndex, CSlot* newElement);
  int Add(CSlot* newElement);
  CSlot* GetForName(CString Name);
  void RemoveAll();
};

///////////////////////////////////////////////////////////// CFrame
//
// 9.      CFrame      - F R A M E
//
/////////////////////////////////////////////////////////////
class CFrame : public CObject
{  DECLARE_SERIAL(CFrame)
public:
  CFrame();  // Construction
// Attributes
  CString m_Name;            // Name of frame
  WORD m_Specimen;           // Number of frame type
  CString m_Comment;         // Comment of frame
  CArraySlots m_ArraySlots;  // Array of slots
  ~CFrame();
  void Serialize(CArchive&);
};

///////////////////////////////////////////////////////////// CMapFrames
//
// 10.   CMapFrames    - Map  of  F R A M E
//
/////////////////////////////////////////////////////////////
class CMapFrames : public CMapStringToOb
{
public:
  CMapFrames();
  DECLARE_DYNCREATE(CMapFrames)
// Operations
public:
  CFrame* GetFrame(int Num);
  void GetNextAssoc(POSITION& rNextPosition, CString& rKey, CFrame*& rValue);
  BOOL Lookup(const char* key, CFrame*& rValue);
  void SetAt(const char* key, CFrame* newValue);
  void RemoveAll();
};


///////////////////////////////////////////////////////////// CArrayProduction
//
// 12.CArrayProduction - Array of P R O D U C T I O N S
//
/////////////////////////////////////////////////////////////
class CArrayProduction : public CObArray
{
public:
  CArrayProduction();
  DECLARE_DYNCREATE(CArrayProduction)
// Accessing elements
  CProduction* GetAt(int nIndex) const;
  void SetAt(int nIndex, CProduction* newElement);
  int Add(CProduction* newElement);
  void RemoveAll();
};

///////////////////////////////////////////////////////////// CStrategy
//
// 13.     CStrategy   - S T R A T E G Y
//
/////////////////////////////////////////////////////////////
class CStrategy : public CObject
{  DECLARE_SERIAL(CStrategy)
public:
  CStrategy();  // Construction
// Attributes
  CString m_Name;                      // Name of strategy
  CString m_Comment;                   // Comment of strategy
  CArrayProduction m_ArrayProduction;  // Array of productions
  ~CStrategy();
  void Serialize(CArchive&);
};

/////////////////////////////////////////////////////////////
//
// 14.  CMapStrategies  - Map  of  S T R A T E G I E S
//
/////////////////////////////////////////////////////////////
class CMapStrategies : public CMapStringToOb
{
public:
  CMapStrategies();
  DECLARE_DYNCREATE(CMapStrategies)
// Operations
public:
  CStrategy* GetStrategy(int Num);
  void GetNextAssoc(POSITION& rNextPosition, CString& rKey, CStrategy*& rValue);
  BOOL Lookup(const char* key, CStrategy*& rValue);
  void SetAt(const char* key, CStrategy* newValue);
  void RemoveAll();
};


///////////////////////////////////////////////////////////// CArrayExpression
//
// 15. CDoubleArray - Array of double value
//
/////////////////////////////////////////////////////////////
class CDoubleArray : public CObArray
{
public:
  CDoubleArray();
  ~CDoubleArray();
  DECLARE_DYNCREATE(CDoubleArray)
// Accessing elements
  double* GetAt(int nIndex) const;
  void SetAt(int nIndex, double newElement);
  void SetAtGrow(int nIndex, double newElement);
  int Add(double newElement);
  void RemoveAll();
};

///////////////////////////////////////////////////////////// CMapValFrame
//
// 16. CMapValFrame - Map of values attributes for Frame
//
/////////////////////////////////////////////////////////////
class CMapValFrame : public CMapStringToString
{
public:
  CMapValFrame() { }

  DECLARE_DYNCREATE(CMapValFrame)
// Operations
public:
  BOOL GetAt(CString key, CString& res);
  BOOL GetAt(CString key, double& d);
  int GetAt(CMapAttributes* m_pMapAttributes, CString key,
            CString& res, double& d);  
            // -1 - bad, 0 - BOOL, 1 - double, 2 - string
  void SetAt(const char* key, double newValue);
  void SetAt(const char* key, CString newValue);
  BOOL  WriteToIni(CString File, CString Section);
  BOOL  ReadFromIni(CString sFile, CString Section, CStringArray &whatAttr);
  BOOL CheckAttr(const char* key, CString Value);
  BOOL CheckAttr(const char* key, double Value);
};

///////////////////////////////////////////////////////////// CBStack
//
// 17. CBStack     -  Stack with integers
//
/////////////////////////////////////////////////////////////
class CBStack : public CByteArray
{
public:
  CBStack();
  DECLARE_DYNCREATE(CBStack)
// Operations
public:
// Accessing elements
  void Push(BYTE El);
  void Clear();
  BOOL Pop(BYTE& El);
};

///////////////////////////////////////////////////////////// CStrStack
//
// 18. CStrStack     -  Stack with strategies names
//
/////////////////////////////////////////////////////////////
class CStrStack : public CStringArray
{
public:
  CStrStack();
  DECLARE_DYNCREATE(CStrStack)
// Operations
public:
// Accessing elements
  void Push(CString El);
  BOOL Pop(CString& El);
  void Clear();
};

///////////////////////////////////////////////////////////// CPNodeArraySmall
//
// 19. CPNodeArraySmall     -  Special array of nodes CPNode
//
/////////////////////////////////////////////////////////////
class CPNodeArraySmall : public CObArray 
{
public:
  CPNodeArraySmall(); 
  DECLARE_DYNCREATE(CPNodeArraySmall)
// Operations
public:
  int Add(CPNode* pNode);
  CPNode* GetAt(int nIndex);

};

///////////////////////////////////////////////////////////// CPNode
//
// 20. CPNode     -  node of SF net
//
/////////////////////////////////////////////////////////////
//Element  EK[i] contain reference to CPNode (it's index in array ???), 
//which is shown in row number i in view
class CPNode : public CObject
{
public:
  CPNode();
  ~CPNode();
  DECLARE_SERIAL(CPNode)

// Attributes
  CSF              *m_pSF;    // Element   -SF
  CString          m_PldName;      // File with PLD  
  CMapValFrame     m_Attr;
  BYTE             m_level;        // Number of elements level
  CPNodeArraySmall m_Children;
  int              m_NumStringInView;

// Operations
public:
  virtual void Serialize(CArchive& ar);   // overridden for document i/o
};

///////////////////////////////////////////////////////////// CPNodeArray
//
// 21. CPNodeArray     -  Array of nodes CPNode
//
/////////////////////////////////////////////////////////////
class CPNodeArray : public CObArray 
{
public:
  CPNodeArray(); 
  DECLARE_DYNCREATE(CPNodeArray)
// Operations
public:
  int Add(CPNode* pNode);
  CPNode* GetAt(int nIndex);  
  void RemoveAll(); 
  CPNode *GetChildWithAttrib(int ActNode, CKnAttr *pKnAttr);

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
};

///////////////////////////////////////////////////////////// CMenuESS
//
// 22.    CMapStringToNode   - Map of strings for nodes
//
/////////////////////////////////////////////////////////////
class CMapStringToNode : public CMapStringToOb
{
public:
  CMapStringToNode();
  DECLARE_DYNCREATE(CMapStringToNode)
// Operations
public:
  void GetNextAssoc(POSITION& rNextPosition, CString& rKey,
                     CPNode*& rValue);
  BOOL Lookup(const char* key, CPNode*& rValue);
  void SetAt(const char* key, CPNode* newValue);
  void RemoveAll();
  void RemoveKey(CString Buf);

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

};

///////////////////////////////////////////////////////////// CMenuESS
//
// 23.      CMenuESS       - menu position in ESS
//
/////////////////////////////////////////////////////////////
class CMenuESS : public CObject
{  DECLARE_SERIAL(CMenuESS)
public:
  CMenuESS();  // Construction
// Attributes
  CString m_Name;        // Name of program modul (key for Map)
  CString m_Label;       // Text of menu position
  HTREEITEM m_hParent;     // Handle of parent menu
  HTREEITEM m_hOwn;        // Own handle
  ~CMenuESS();
  void Serialize(CArchive&);
};

///////////////////////////////////////////////////////////// CArrayMenuESS
//
// 24.  CArrayMenuESS     - Array of MenuESS positions
//
/////////////////////////////////////////////////////////////
class CArrayMenuESS : public CObArray
{
public:
  CArrayMenuESS();
  DECLARE_DYNCREATE(CArrayMenuESS)
// Accessing elements
  CMenuESS* GetAt(int nIndex) const;
  int Add(CMenuESS* newElement);
  void RemoveAll();
};



//--------------------------------------------------------------------
BOOL ReadDoc(CString FileName, CMapAttributes* pA, CMapProgModuls* pM,
             CMapStrategies* pS, CMapFrames* pF, CWinApp *pApp);

#endif //__OBJECTS.H__
