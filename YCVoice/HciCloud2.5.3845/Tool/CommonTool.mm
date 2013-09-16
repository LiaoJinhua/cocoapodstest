#include "CommonTool.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;

CFileBuf::CFileBuf()
{
	m_pBuf = NULL;
	m_nLen = 0;
}

CFileBuf::~CFileBuf()
{
	Free();
}

bool CFileBuf::Load(const char * pszLibName, int nExtraBytes)
{
	FILE * fp = fopen(pszLibName, "rb");
	if (fp == NULL)
		return false;

	fseek(fp,0,SEEK_END);
	m_nLen=ftell(fp);
	fseek(fp,0,SEEK_SET);

	if (m_nLen == 0)
	{
		fclose(fp);
		return false;
	}

	m_pBuf = (unsigned char*)malloc(m_nLen + nExtraBytes);
	if (m_pBuf == NULL)
	{
		fclose(fp);
		return false;
	}

	fread( m_pBuf, 1, m_nLen, fp);
	if (ferror(fp) != 0)
	{
		fclose(fp);
		free(m_pBuf);
		m_pBuf = NULL;
		return false;
	}

	fclose(fp);

	if (nExtraBytes != 0)
	{
		memset(m_pBuf + m_nLen, 0, nExtraBytes);
		m_nLen += nExtraBytes;
	}
	return true;
}

void CFileBuf::Free()
{
	if (m_pBuf != NULL)	
	{					
		free(m_pBuf);
		m_pBuf = NULL;
	}

	m_nLen = 0;
}


std::string GetIosAppDocumentPath(void)
{	
	NSString* nsPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];

	std::string strRet =  NsStrToString(nsPath);

	return strRet;
}

std::string NsStrToString(NSString* nsStr)
{
	std::string strRet = std::string("");
	if (nsStr == nil) {
		return strRet;
	}
	strRet = [nsStr UTF8String];
	return strRet;
}


std::string GetIosResDataPath(NSString*fileName,NSString*fileType)
{
    NSString *nsSoucePath =[[NSBundle mainBundle] pathForResource:fileName ofType:fileType];
	std::string strSourcePath = NsStrToString(nsSoucePath);
	if ((fileName == nil) && (fileType == nil)) {
		return strSourcePath;
	}
	std::string strFileName = NsStrToString(fileName);
	std::string strFileType = NsStrToString(fileType);
	
	if ((fileName != nil) && (fileType == nil)) {
		 strFileName = std::string("/") + strFileName;
	}else if ((fileName != nil) && (fileType != nil)) {
		strFileName = std::string("/") + strFileName + std::string(".") + strFileType;
	}else if ((fileName == nil) && (fileType != nil)) {
		strFileName = std::string("/");
	}

	size_t unpos = strSourcePath.rfind(strFileName.c_str());
	strSourcePath = strSourcePath.substr(0,unpos);
	return strSourcePath;
}
#define PALTFORM_ID_STR "platformId="
#define APP_NO_STR "appNo="
#define DEVELOPER_ID_STR "developerId="
#define DEVELOPER_KEY_STR "developerKey="
#define CLOUD_URL_STR "cloudUrl="
bool GetAccountInfo(NSString*fileName,NSString*fileType,string &strAccountInfo)
{	
	strAccountInfo.clear();
	ifstream fin;
	fin.open(GetIosFullResDataPath(fileName,fileType).c_str());
	
	if (!fin)
	{
		printf("get account info failed \n\t may be the file %s not exist!\n",GetIosFullResDataPath(fileName,fileType).c_str());
		return false;
	}
	
	string strTmp;
	while(getline(fin,strTmp))
	{
		strTmp = trim(strTmp);
		if (strTmp[0] == '#' || strTmp.empty())
		{
			continue;
		}
		if (
			(strTmp.find(PALTFORM_ID_STR) == 0 && strTmp.length() > strlen(PALTFORM_ID_STR))
			|| (strTmp.find(APP_NO_STR) == 0 && strTmp.length() > strlen(APP_NO_STR))
			|| (strTmp.find(DEVELOPER_ID_STR) == 0 && strTmp.length() >strlen(DEVELOPER_ID_STR) )
			|| (strTmp.find(DEVELOPER_KEY_STR) == 0 && strTmp.length() >strlen(DEVELOPER_KEY_STR) )
			|| (strTmp.find(CLOUD_URL_STR) == 0 && strTmp.length() >strlen(CLOUD_URL_STR) )
			)
		{
			strAccountInfo += strTmp;
			strAccountInfo += ",";	
		}
		else
		{
			printf("get account info failed \n\t account info(%s,%s,%s,%s,%s) \n\t some record info may be empty,please check the file %s!\n",
				   PALTFORM_ID_STR,APP_NO_STR,DEVELOPER_ID_STR,DEVELOPER_KEY_STR,CLOUD_URL_STR,GetIosFullResDataPath(fileName,fileType).c_str());
			return false;
		}
	}
	
	fin.close();
	if (strAccountInfo.find(PALTFORM_ID_STR) == string::npos 
		|| strAccountInfo.find(APP_NO_STR) == string::npos 
		|| strAccountInfo.find(DEVELOPER_ID_STR) == string::npos 
		|| strAccountInfo.find(DEVELOPER_KEY_STR) == string::npos 
		|| strAccountInfo.find(CLOUD_URL_STR) == string::npos 
		)
	{
		printf("get account info failed \n\t account info(%s,%s,%s,%s,%s) \n\t some record info may be missed,please check the file %s!\n",
			   PALTFORM_ID_STR,APP_NO_STR,DEVELOPER_ID_STR,DEVELOPER_KEY_STR,CLOUD_URL_STR,GetIosFullResDataPath(fileName,fileType).c_str());
		return false;
	}
	
	return true;
}

std::string GetIosFullResDataPath(NSString *fileName, NSString *fileType)
{
	NSString *nsSoucePath =[[NSBundle mainBundle] pathForResource:fileName ofType:fileType];
	std::string strSourcePath = NsStrToString(nsSoucePath);
	return strSourcePath;
}

string trim(const string& str) {
	string t = str;
	t.erase(0, t.find_first_not_of(" \t\n\r"));
	t.erase(t.find_last_not_of(" \t\n\r") + 1);
	return t;
}