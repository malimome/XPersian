#include <list>
#include <tchar.h>

using namespace std;
typedef std::list<const char*>    str_list;
typedef std::list<const TCHAR*>    wstr_list;

const int AUDIO_PROP      = 0;
const int CUSTOMIZE       = 1;
const int HARDWARE        =	2;
const int PRINTER         =	3;
const int SYSTEM_PROP     =	4;
const int ACCESS_OPT	  =	5;
const int CD_WRITE		  = 6;
const int CONFIRM		  =	7;
const int DISK_CLEAN	  =	8;
const int DISP_PROP		  = 9;
const int FOLDER_OPT	  =	10;
const int INTERNET_PROP	  = 11;
const int MOUSE_PROP	  =	12;
const int CONNECT_WIZ	  =	13;
const int MODEM_OPT		  = 14;
const int POWER_OPT		  = 15;
const int LANGUAGE_OPT	  = 16;
const int SEARCH_RES	  =	17;
const int FIREWALL		  =	18;
const int SECURITY		  =	19;
const int DEFENDER		  = 20;
const int PROPERTIES	  =	68;
const int SFSDSK		  =	69;
const int MISC			  = 70;

void Load_Captions  (wstr_list& e,wstr_list& p);

void Load_Audio     (str_list& e,wstr_list& p);
void Load_CUSTOMIZE (str_list& e,wstr_list& p);
void Load_HARDWARE  (str_list& e,wstr_list& p);
void Load_PRINTER   (str_list& e,wstr_list& p);
void Load_SYSTEM    (str_list& e,wstr_list& p);
void Load_ACCESS    (str_list& e,wstr_list& p);
void Load_CD        (str_list& e,wstr_list& p);
void Load_CONFIRM   (str_list& e,wstr_list& p);
void Load_DISK      (str_list& e,wstr_list& p);
void Load_DISP      (str_list& e,wstr_list& p);
void Load_FOLDER    (str_list& e,wstr_list& p);
void Load_INTERNET  (str_list& e,wstr_list& p);
void Load_MOUSE     (str_list& e,wstr_list& p);
void Load_CONNECT   (str_list& e,wstr_list& p);
void Load_MODEM     (str_list& e,wstr_list& p);
void Load_POWER     (str_list& e,wstr_list& p);
void Load_LANGUAGE  (str_list& e,wstr_list& p);
void Load_SEARCH    (str_list& e,wstr_list& p);
void Load_FIREWALL  (str_list& e,wstr_list& p);
void Load_SECURITY  (str_list& e,wstr_list& p);
void Load_DEFENDER  (str_list& e,wstr_list& p);
void Load_PROPERTIES(str_list& e,wstr_list& p);
void Load_SFSDSK    (str_list& e,wstr_list& p);
void Load_MISC      (str_list& e,wstr_list& p);