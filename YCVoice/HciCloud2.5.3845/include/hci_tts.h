/** 
 * @file    hci_tts.h 
 * @brief   HCI_TTS SDK 头文件  
 */  

#ifndef _hci_tts_header_
#define _hci_tts_header_

#include "hci_sys.h"

#ifdef __cplusplus
extern "C"		
{
#endif

/** @defgroup HCI_TTS 灵云TTS能力API */
/* @{ */


//////////////////////////////////////////////////////////////////////////
// 常量定义

#define TTS_CAP_PROP_SPEAKER	"speaker"		///< TTS能力的属性名称：识别模式。
#define TTS_CAP_PROP_GENDER		"gender"		///< TTS能力的属性名称：音色。值：男声(male), 女声(female), 童声(child)
#define TTS_CAP_PROP_LANG		"lang"			///< TTS能力的属性名称：语言
#define TTS_CAP_PROP_DOMAIN		"domain"		///< TTS能力的属性名称：领域

//////////////////////////////////////////////////////////////////////////
// 数据结构定义

/**
 * @brief	TTS 合成结果
 */
typedef struct _tag_TTS_SYNTH_RESULT 
{
	/// 数据缓冲区指针，数据格式由合成时候的配置决定
	void * pvVoiceData;

	/// 数据缓冲区长度，以字节为单位
	unsigned int uiVoiceSize;

	/// 本段数据对应的文本内容的起始指针
	char * pszText;

	/// 本段数据对应的文本长度，字节为单位
	unsigned int uiTextLen;
	
	/// 是否还有数据等待合成
	bool bMore;
} TTS_SYNTH_RESULT;

//////////////////////////////////////////////////////////////////////////
// 回调函数定义

/**  
 * @brief	语音合成回调函数
 * @param	pvUserParam			用户自定义回调指针
 * @param	psTtsSynthResult	合成结果的数据结构指针
 * @param	hciErrCode			错误码
 * @return	是否继续合成，返回true表示继续合成，返回false表示中止合成
 *
 * @note
 * psTtsSythResult中的数据在本回调函数返回后，不保证继续有效，因此用户如果需要保留数据进行处理，
 * 需要自行将数据拷贝出去。
 */ 
typedef bool (HCIAPI * Callback_TtsSynth)(
		_OPT_ _IN_ void * pvUserParam,
		_MUST_ _IN_ TTS_SYNTH_RESULT * psTtsSynthResult,
		_MUST_ _IN_ HCI_ERR_CODE  hciErrCode
		);

//////////////////////////////////////////////////////////////////////////
// 接口API函数定义

/**  
 * @brief	灵云TTS能力 初始化
 * @param	pszConfig		初始化配置串，ASCII字符串，可为NULL或以'\0'结束
 * @return
 * @n
 *	<table>
 *		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
 *		<tr><td>@ref HCI_ERR_SYS_NOT_INIT</td><td>HCI SYS 尚未初始化</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_ALREADY_INIT</td><td>已经初始化过了</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_INVALID</td><td>配置参数有误，如设定值非法、或格式错误等</td></tr>
 *		<tr><td>@ref HCI_ERR_LOCAL_LIB_MISSING</td><td>本地能力引擎缺失必要的库资源</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_DATAPATH_MISSING</td><td>本地引擎资源路径未配置</td></tr>
 *		<tr><td>@ref HCI_ERR_OUT_OF_MEMORY</td><td>分配内存失败</td></tr>
 *		<tr><td>@ref HCI_ERR_CAPKEY_NOT_FOUND</td><td>未找到授权能力项</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_CAPKEY_NOT_MATCH</td><td>能力与Key不匹配</td></tr>
 *	</table>
 *
 * @par 配置串定义：
 * 配置串是由"字段=值"的形式给出的一个字符串，多个字段之间以','隔开。字段名不分大小写。
 * @n@n
 *	<table>
 *		<tr>
 *			<td><b>字段</b></td>
 *			<td><b>取值或示例</b></td>
 *			<td><b>缺省值</b></td>
 *			<td><b>含义</b></td>
 *			<td><b>详细说明</b></td>
 *		</tr>
 *		<tr>
 *			<td>dataPath</td>
 *			<td>opt/myapp/tts_data/</td>
 *			<td>空</td>
 *			<td>语音合成本地资源所在路径</td>
 *			<td>如果不使用本地语音合成能力，可以不传此项</td>
 *		</tr>
 *		<tr>
 *			<td>initCapKeys</td>
 *			<td>tts.local.xixi;tts.local.haobo</td>
 *			<td>空</td>
 *			<td>初始化所需的本地能力</td>
 *			<td>多个能力以';'隔开，如果传入云端能力key，简单忽略。如果不使用本地语音合成能力，可以不传此项</td>
 *		</tr>
 *		<tr>
 *			<td>codecPath</td>
 *			<td>opt/myapp/asr_data</td>
 *			<td>空</td>
 *			<td>语音编解码动态库所在路径</td>
 *			<td>已废弃，现在只要所有动态库在一个目录下即可</td>
 *		</tr>
 *		<tr>
 *			<td>fileFlag</td>
 *			<td>none, android_so</td>
 *			<td>none</td>
 *			<td>获取本地文件名的特殊标记</td>
 *			<td>参见下面的注释</td>
 *		</tr>
 *	</table>
 *
 *  <b>Android特殊配置</b>
 *  @n
 *  当fileFlag为android_so时，加载本地资源文件时会将正常的库文件名更改为so文件名进行加载。
 *  例如，当使用的库为file.dat时，则实际打开的文件名为libfile.dat.so，这样在Android系统下，
 *  开发者可以按照此规则将本地资源改名后, 放在libs目录下打包入apk。在安装后，这些资源文件
 *  就会放置在/data/data/包名/lib目录下。则可以直接将dataPath配置项指为此目录即可。
 */

HCI_ERR_CODE HCIAPI hci_tts_init(
		_MUST_ _IN_ const char * pszConfig
		);

/**  
 * @brief	开始会话
 * @param	pszConfig		会话配置串，ASCII字符串，以'\0'结束
 * @param	pnSessionId		成功时返回会话ID
 * @return
 * @n
 *	<table>
 *		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_NOT_INIT</td><td>HCI TTS尚未初始化</td></tr>
 *		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>传入的参数不合法</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_INVALID</td><td>配置参数有误，如设定值非法、或格式错误等</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_CAPKEY_MISSING</td><td>缺少必需的capKey配置项</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_CAPKEY_NOT_MATCH</td><td>能力与Key不匹配</td></tr>
 *		<tr><td>@ref HCI_ERR_TOO_MANY_SESSION</td><td>创建的Session数量超出限制(256),本地能力需要打开本地资源文件，最终能够成功打开的本地Session个数与本地系统对同时打开文件个数的限制有关。</td></tr>
 *		<tr><td>@ref HCI_ERR_OUT_OF_MEMORY</td><td>分配内存失败</td></tr>
 *		<tr><td>@ref HCI_ERR_CAPKEY_NOT_FOUND</td><td>未找到授权能力项</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_DATAPATH_MISSING</td><td>本地引擎资源路径未配置</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_ENGINE_SESSION_START_FAILED</td><td>TTS本地引擎开启会话失败</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_UNSUPPORT</td><td>配置参数，引擎不支持</td></tr>
 *		<tr><td>@ref HCI_ERR_LOCAL_LIB_MISSING</td><td>本地能力引擎缺失必要的库资源</td></tr>
 *	</table>
 *
 *
 * @par 配置串定义：
 * 配置串是由"字段=值"的形式给出的一个字符串，多个字段之间以','隔开。字段名不分大小写。
 * @n@n
 *	<table>
 *		<tr>
 *			<td><b>字段</b></td>
 *			<td><b>取值或示例</b></td>
 *			<td><b>缺省值</b></td>
 *			<td><b>含义</b></td>
 *			<td><b>详细说明</b></td>
 *		</tr>
 *		<tr>
 *			<td>capKey</td>
 *			<td>tts.cloud.ZhangNan</td>
 *			<td>无</td>
 *			<td>语音合成能力key</td>
 *			<td>参见 @ref hci_tts_page 。每个session只能定义一种能力，并且过程中不能改变。</td>
 *		</tr>
 *	</table>
 * @n@n
 * 另外，这里还可以传入合成的配置项，作为默认配置项。参见 hci_tts_synth() 。
 */
HCI_ERR_CODE HCIAPI hci_tts_session_start(
		_MUST_ _IN_ const char * pszConfig,
		_MUST_ _OUT_ int * pnSessionId
		);

/**  
 * @brief	进行语音合成操作
 * @param	nSessionId		会话ID
 * @param	pvText			要合成的文本数据，UTF-8编码，以'\0'结束
 * @param	pszConfig		合成参数配置串，ASCII字符串，可为NULL或以'\0'结束
 * @param	pfnTtsSynth		语音合成结果回调函数
 * @param	pvUserParam		用户自定义回调数据指针
 * @return
 * @n
 *	<table>
 *		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_NOT_INIT</td><td>HCI TTS尚未初始化</td></tr>
 *		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>传入的参数不合法</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_INVALID</td><td>配置参数有误，如设定值非法、或格式错误等</td></tr>
 *		<tr><td>@ref HCI_ERR_SESSION_INVALID</td><td>传入的Session无效</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_SESSION_BUSY</td><td>TTS当前会话正忙</td></tr>
 *		<tr><td>@ref HCI_ERR_OUT_OF_MEMORY</td><td>分配内存失败</td></tr>
 *		<tr><td>@ref HCI_ERR_CAPKEY_NOT_FOUND</td><td>未找到授权能力项</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_UNSUPPORT</td><td>配置参数，引擎不支持</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_ENGINE_FAILED</td><td>本地引擎合成失败</td></tr>
 *		<tr><td>@ref HCI_ERR_SERVICE_CONNECT_FAILED</td><td>连接服务器失败，服务器无响应</td></tr>
 *		<tr><td>@ref HCI_ERR_SERVICE_TIMEOUT</td><td>服务器访问超时</td></tr>
 *		<tr><td>@ref HCI_ERR_SERVICE_DATA_INVALID</td><td>服务器返回的数据格式不正确</td></tr>
 *		<tr><td>@ref HCI_ERR_SERVICE_RESPONSE_FAILED</td><td>服务器返回合成失败</td></tr>
 *		<tr><td>@ref HCI_ERR_LOAD_CODEC_DLL</td><td>加载codec编码库失败</td></tr>
 *	</table>
 *
 * @par 配置串定义：
 * 配置串是由"字段=值"的形式给出的一个字符串，多个字段之间以','隔开。字段名不分大小写。
 * @n@n
 *	<table>
 *		<tr>
 *			<td><b>字段</b></td>
 *			<td><b>取值或示例</b></td>
 *			<td><b>缺省值</b></td>
 *			<td><b>含义</b></td>
 *			<td><b>详细说明</b></td>
 *		</tr>
 *		<tr>
 *			<td>pitch</td>
 *			<td>0 - 9</td>
 *			<td>5</td>
 *			<td>合成语音的基频</td>
 *			<td>数字越大基频越高</td>
 *		</tr>
 *		<tr>
 *			<td>volume</td>
 *			<td>0 - 9</td>
 *			<td>5</td>
 *			<td>合成语音的音量</td>
 *			<td>数字越大音量越大</td>
 *		</tr>
 *		<tr>
 *			<td>speed</td>
 *			<td>0 - 9</td>
 *			<td>5</td>
 *			<td>合成语音的语速</td>
 *			<td>数字越大语速越快</td>
 *		</tr>
 *		<tr>
 *			<td>audioFormat</td>
 *			<td>pcm16k16bit</td> 
 *			<td>auto</td>
 *			<td>输出音频格式</td>
 *			<td>云端引擎支持的音频格式:<br/>
 *				auto, vox6k4bit, vox8k4bit<br/>
 *				alaw8k8bit, ulaw8k8bit<br/>
 *				pcm8k8bit, pcm8k16bit<br/>
 *				pcm11k8bit, pcm11k16bit<br/>
 *				pcm16k8bit, pcm16k16bit<br/>
 *				mp3<br/>
 *				本地引擎支持的音频格式:<br/>
 *				auto, pcm8k8bit, pcm8k16bit<br/>
 *				pcm11k8bit, pcm11k16bit<br/>
 *				pcm16k8bit, pcm16k16bit<br/>
 *				alaw8k8bit, ulaw8k8bit<br/></td> 
 *		</tr>
 *		<tr>
 *			<td>encode</td>
 *			<td>none, opus, speex</td>
 *			<td>opus</td>
 *			<td>使用的编码格式，只对云端合成有效，本地合成忽略该配置</td>
 *			<td>表示云端合成传输的时候采用的编码格式，当audioFormat为vox6k4bit/vox8k4bit/mp3时，
 *              不支持使用opus,speex（直接忽略配置），其余均可采用opus,speex进行压缩传输，以减少数据流量</td>
 *		</tr> 
 *		<tr>
 *			<td>encLevel</td>
 *			<td>0-10</td>
 *			<td>7</td>
 *			<td>压缩等级，只对云端合成有效，本地合成忽略该配置</td>
 *			<td></td>
 *		</tr> 
 *	</table>
 * @n@n
 * 以下配置，与能力相关，并非所有的能力都支持，参见《灵云能力列表》文档。具体使用上可咨询捷通华声公司。
 *  <table>
 *		<tr>
 *			<td><b>字段</b></td>
 *			<td><b>取值或示例</b></td>
 *			<td><b>缺省值</b></td>
 *			<td><b>含义</b></td>
 *			<td><b>详细说明</b></td>
 *		</tr>
 *		<tr>
 *			<td>puncMode</td>
 *			<td>off, on, off_rtn, on_rtn</td>
 *			<td>off</td>
 *			<td>标点符号读法</td>
 *			<td>off: 不合成标点符号，自动判断回车换行是否分隔符<br/>
 *				on: 合成标点符号，自动判断回车换行是否分隔符<br/>
 *				off_rtn: 不读符号，强制将回车换行作为分隔符<br/>
 *				on_rtn: 读符号，强制将回车换行作为分隔符</td>
 *		</tr>
 *		<tr>
 *			<td>speedUp</td>
 *			<td>auto,0,1,2,3</td>
 *			<td>auto</td>
 *			<td>引擎加速设置</td>
 *			<td>auto: Android自动判断cpu设置，其他平台默认为0<br/>
 *				0: 不进行加速</td>
 *				1: 加速，不优化</td>
 *				2: 加速，优化%30</td>
 *				3: 加速，优化%50</td>
 *		</tr>
 *		<tr>
 *			<td>namePolyphone</td>
 *			<td>off, on</td>
 *			<td>on</td>
 *			<td>姓氏多音字开关</td>
 *			<td>off: 关闭<br/>
 *				on: 打开</td>
 *		</tr>
 *		<tr>
 *			<td>speedMode</td>
 *			<td>single, double</td>
 *			<td>single</td>
 *			<td>语速加倍设置</td>
 *			<td>single: 单倍语速，通用设置<br/>
 *				double: 双倍语速，一般为盲人使用</td>
 *		</tr>
 *		<tr>
 *			<td>silenceMode</td>
 *			<td>short, long</td>
 *			<td>long</td>
 *			<td>静音缩短设置</td>
 *			<td>long: 静音较长<br/>
 *				short: 静音短，一般盲人使用</td>
 *		</tr>
 *		<tr>
 *			<td>symbolFilter</td>
 *			<td>off, on</td>
 *			<td>on</td>
 *			<td>标点符号控制参数</td>
 *			<td>off: 关闭符号忽略：不对符号进行过滤，按照正常的断句与符号处理方式进行处理<br/>
 *				on: 开启符号忽略：连续的符号，包括全半角句号、逗号、问好、感叹号、空格，均按照过滤后保留一个符号处理；此为默认设置</td>
 *		</tr>
 *		<tr>
 *			<td>specialOne</td>
 *			<td>yi1,yao1</td>
 *			<td>无</td>
 *			<td>'1'的读法</td>
 *			<td>不设置该项则自动判断，'1'的读法，分别为yi1和yao1; 此项仅在电报读法下有效</td>
 *		</tr>
 *		<tr>
 *			<td>specialTwo</td>
 *			<td>er4,liang3</td>
 *			<td>无</td>
 *			<td>'2'的读法</td>
 *			<td>不设置该项则自动判断，'2'的读法，分别为er4和liang3; 此项仅在数目读法下有效</td>
 *		</tr>
 *		<tr>
 *			<td>digitMode</td>
 *			<td>auto_number, auto_telegram, number, telegram</td>
 *			<td>auto_number</td>
 *			<td>数字阅读方式</td>
 *			<td>auto_number: 自动方式，即根据上下文自动选择电报方式或数目方式合成，如果无法判断，则使用数目方式。<br /> 
 *				auto_telegram: 自动方式，即根据上下文自动选择电报方式或数目方式合成，如果无法判断，则使用电报方式。<br />
 *				number: 数目方式，即连续的数字合成。例如：1997读成“一千九百九十七”。 <br />
 *				telegram: 电报方式，即一个数字一个数字合成。例如：1997读成“一九九七”。 </td>
 *		</tr>
 *		<tr>
 *			<td>engMode</td>
 *			<td>auto, english, letter</td>
 *			<td>auto</td>
 *			<td>英文阅读方式</td>
 *			<td>auto: 自动判断。 <br /> 
 *				english: 强制所有英文用字母或自录音词汇合成。<br /> 
 *				letter: 强制所有英文按照单字母方式合成。 </td> 
 *		</tr>
 *		<tr>
 *			<td>voiceStyle</td>
 *			<td>clear, vivid, normal, plain</td>
 *			<td>normal</td>
 *			<td>朗读风格</td>
 *			<td>clear: 清晰<br/>
 *				vivid: 生动<br/>
 *				normal: 抑扬顿挫 <br/>
 *				plain: 平稳庄重 </td> 
 *		</tr>
 *		<tr>
 *			<td>soundEffect</td>
 *			<td>base,reverb,echo,chorus,nearfar,robot</td>
 *			<td>base</td>
 *			<td>音效设置</td>
 *			<td>base: 无音效<br/>
 *				reverb: 混响<br/>
 *				echo: 回声<br/>
 *				chorus: 合唱<br/>
 *				nearfar: 忽远忽近<br/>
 *				robot: 机器人</td> 
 *		</tr>
 *		<tr>
 *			<td>mixSound</td>
 *			<td>1 - 9</td>
 *			<td>1</td>
 *			<td>混响时长</td>
 *			<td>数字越大混音时间越长,该配置只有在soundEffect设置为echo时生效</td>
 *		</tr>
 *		<tr>
 *			<td>gainFactor</td>
 *			<td>2 - 8</td>
 *			<td>2</td>
 *			<td>增益系数</td>
 *			<td>数字越大增益效果越明显，同时调大增益系数与音量，可以有效提升引擎合成的整体音量输出</td>
 *		</tr>
 *		<tr>
 *			<td>domain</td>
 *			<td>finance</td>
 *			<td>空</td>
 *			<td>合成使用的领域</td>
 *			<td>使用指定领域的音库进行合成。如果capKey的能力未指定特定领域，
 *				则可以使用domain进行进一步指定领域。如果capKey的能力已经指定了特定领域，
 *				则该配置项无效。<br/>
 *				common      通用领域，新闻<br/>
 *				finance     金融证券<br/>
 *				weather     天气预报<br/>
 *				sports      体育赛事<br/>
 *				traffic     公交信息<br/>
 *				travel      旅游餐饮<br/>
 *				carborne    汽运<br/>
 *				queue       排队<br/>
 *				song        点歌<br/>
 *				insurance	保险<br/>
 *				voyage      航空<br/>
 *				revenue     税务<br/>
 *				elecpower   电力<br/>
 *				message     短信<br/>
 *				finainsu    金融保险<br/>
 *				telecom     电信<br/>
 *				custom      自定义<br/>
 *				此项可选。具体可用的领域可以在开发时咨询捷通华声。</td>
 *		</tr>
 *	</table>
 * @n@n
 * 这里没有定义的配置项，会使用 session_start 时的定义。如果 session_start 时也没有定义，则使用缺省值
 *
 * @note
 * 1. 本函数通过回调函数的方式给出数据，但仍然是同步方式，也即只有所有数据都通过回调函数给出后，
 * 本函数才会返回;@n
 * 2. 本函数是分段合成，TTS_SYNTH_RESULT中的pszText为当前正在合成文本段的首地址，
 * 如果当前文本段的合成数据需要多次回调，则只在第一次回调时给出该文本段的信息，
 * 其余回调中，该地址为NULL;@n
 * 3. 合成的过程中，如果当前文本段为空行等情况，回调可能没有声音数据返回，即pvVoiceData可能为NULL。<br/>
 */ 
HCI_ERR_CODE HCIAPI hci_tts_synth(
		_MUST_ _IN_ int nSessionId,
		_MUST_ _IN_ char * pvText,			
		_OPT_ _IN_ const char * pszConfig,
		_MUST_ _IN_ Callback_TtsSynth pfnTtsSynth,
		_OPT_ _IN_ void * pvUserParam
		);

/**  
 * @brief	结束会话
 * @param	nSessionId		会话ID
 * @return
 * @n
 *	<table>
 *		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_NOT_INIT</td><td>HCI TTS尚未初始化</td></tr>
 *		<tr><td>@ref HCI_ERR_SESSION_INVALID</td><td>传入的Session非法</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_SESSION_BUSY</td><td>HCI TTS忙，尚有hci_tts_session_synth执行中</td></tr>
 *	</table>
 */ 
HCI_ERR_CODE HCIAPI hci_tts_session_stop(
		_MUST_ _IN_ int nSessionId
		);

/**  
 * @brief	灵云TTS能力 反初始化
 * @return
 * @n
 *	<table>
 *		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_NOT_INIT</td><td>HCI TTS尚未初始化</td></tr>
 *		<tr><td>@ref HCI_ERR_ACTIVE_SESSION_EXIST</td><td>尚有未stop的Sesssion，无法结束</td></tr>
 *	</table>
 */ 
HCI_ERR_CODE HCIAPI hci_tts_release();

/* @} */
//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
};
#endif

#endif // _hci_tts_header_
