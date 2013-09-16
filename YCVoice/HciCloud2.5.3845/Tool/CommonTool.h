#ifndef __common_tool_header__
#define __common_tool_header__
#include <string>

class CFileBuf
{
public:
	unsigned char * m_pBuf;
	int	   m_nLen;

public:
	CFileBuf();
	~CFileBuf();
	virtual bool Load(const char * pszLibName, int nExtraBytes = 0);
	virtual void Free();
};

std::string GetIosAppDocumentPath(void);

std::string NsStrToString(NSString* nsStr); 

//获取授权信息
bool GetAccountInfo(NSString*fileName,NSString*fileType,std::string &strAccountInfo);

//获取不带有文件名的文件路径
std::string GetIosResDataPath(NSString*fileName,NSString*fileType);

//获取带有文件名的全路径
std::string GetIosFullResDataPath(NSString *fileName, NSString *fileType);

std::string trim(const std::string& str);
#endif 
