#ifndef INIMAP
#define INIMAP
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

//-------------------------------------------------------------------------
// ����� ������� ���� ������ Map, ���������� ���������� ����� �������������
//-------------------------------------------------------------------------
// ������ 1.0.   20.01.2014
//-------------------------------------------------------------------------

class IniMap
{

public:
  IniMap() {}
  IniMap(string FileName);  // ��������� ����� ������ ��� ���������� ���� �������������

// Attributes
  string FileName; // ��� ini-�����
  map <string, string> dbMap;    // ��������� ��� �������� ���������� - ����
                                 // ���� ������� �� ���� ������, ����������� �������� ":"
                                 // <��� ������>:<��� ���������>  
  map <string, string>::iterator Idb; // �������� �� ����

// Methods
  bool GetDbMap(string FileName);  // �������� ���������� ���� �� ���������� �����
  bool PutDbMap(string FileName);  // ��������� ���������� ���� � ��������� �����

// ������� ������ �� �����: Sect - ��� ������ �����
//                          Key - ��� ��������� 
//                          Value - ��������� �������� ���������,
//                          Def - �������� ��������� �� ���������
// �������: 0 - ������������ �������� �� ���������
//          1 - �������� ������� �� ����

  //����� �� ���� ��������� �������� �� ����� ������ � �����
inline bool GetParam(string Sect, string Key, string& Value, string Def) 
            { return GetParam(Sect+":"+Key,Value, Def); }
  //����� �� ���� �������� (����������� �����) �� ����� ������ � �����
  bool GetParam(string Sect, string Key, double& Value, double Def) 
            { return GetParam(Sect+":"+Key, Value, Def); }
  //����� �� ���� �������� (����� �����) �� ����� ������ � �����
  bool GetParam(string Sect, string Key, int& Value, int Def)    
            { return GetParam(Sect+":"+Key, Value, Def); }

// ������� ������ �� �������� �����: S_Key - ��� ��������� � ����� (������:����)

  //����� �� ���� ��������� �������� �� "�������� �����" (������:����)
  bool GetParam(string S_Key, string& Value, string Def);  
  //����� �� ���� �������� (����������� �����) �� "�������� �����" (������:����)
  bool GetParam(string S_Key, double& Value, double Def);  
  //����� �� ���� �������� (������� �����) �� "�������� �����" (������:����)
  bool GetParam(string S_Key, int& Value, int Def);     
  
  // ������� �������� ���� ������������ � ���� ������
  void GetSectionsName(vector <string>& SectList); 

  // ������ ������ �� �����: Sect - ��� ������ �����
  //                          Key - ��� ��������� 
  //                          Value - ��������� �������� ���������

  //�������� � ���� ��������� �������� �� ����� ������ � �����
inline  bool SetParam(string Sect, string Key, string Value) 
            { return SetParam(Sect+":"+Key,Value); }
  //�������� � ���� �������� (����������� �����) �� ����� ������ � �����
inline  bool SetParam(string Sect, string Key, double Value) 
            { return SetParam(Sect+":"+Key,Value); }
  //�������� � ���� �������� (����� �����) �� ����� ������ � �����
inline  bool SetParam(string Sect, string Key, int Value)    
            { return SetParam(Sect+":"+Key,Value); }

  // ������� ������ �� �������� �����: S_Key - ��� ��������� � ����� (������:����)

  bool SetParam(string S_Key, string Value);  
  //�������� � ���� ��������� �������� �� "�������� �����" (������:����)
  bool SetParam(string S_Key, double Value);  
  //�������� � ���� �������� (����������� �����) �� "�������� �����" (������:����)
  bool SetParam(string S_Key, int Value);     
  //�������� � ���� �������� (������� �����) �� "�������� �����" (������:����)


  // ������
inline  bool FindKey(string Sect, string Key) { return FindKey(Sect+":"+Key); }

  bool FindKey(string S_Key);
};

#endif  //INIMAP