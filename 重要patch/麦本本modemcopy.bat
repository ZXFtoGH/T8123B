@ECHO OFF

set project_name=t8123b_l40mmeim_T1095_64

set "source_path=%~dp0out\target\product\%project_name%\merged"
set "source_path_sub=%~dp0out\target\product\%project_name%\merged\download_agent"

set out_path=\\192.168.2.170\�����\��ʱ�ļ�\zhangxiangfei\T8123B_MBB\merged
set out_path_sub=\\192.168.2.170\�����\��ʱ�ļ�\zhangxiangfei\T8123B_MBB\merged\download_agent

mkdir %out_path%
mkdir %out_path_sub%

TITLE %project_name% һ������
color 3f

:MENU
CLS
ECHO.
ECHO.                    %project_name% һ����������
ECHO.***************************************************************************
ECHO.
ECHO.      ������ͣ�%project_name%
ECHO.      Դ·����%source_path%
ECHO.      Ŀ��·�� ��%out_path%
ECHO.           
ECHO.      ��ѡ�񿽱����ݣ�
ECHO.          1. ֻ���� pl lk boot
ECHO.          2. ����ȫ��
ECHO.                    
ECHO.           
ECHO.***************************************************************************
ECHO.

set choice=
set /p choice=��ֱ�������Ӧ���ֻس���
if not "%choice%"=="" set choice=%choice:~0,1%
if /i "%choice%"=="1" goto CP_LK_BOOT
if /i "%choice%"=="2" goto CP_ALL

ECHO.
ECHO.������Ч������������...
timeout /t 2 /nobreak >NUL
ECHO.
goto MENU


:CP_ALL
CLS
ECHO.
ECHO.               %project_name% һ����������
ECHO.*************************************************************
ECHO.      
ECHO.       ��ʼ���� all......
ECHO.                     
ECHO.*************************************************************
ECHO.

copy  %source_path%\boot.img %out_path%\ /y
copy  %source_path%\dtbo.img %out_path%\ /y
copy  %source_path%\gz.img %out_path%\ /y
copy  %source_path%\lk.img %out_path%\ /y
copy  %source_path%\logo.bin %out_path%\ /y
copy  %source_path%\preloader_t8123b_l40mmeim_T1095_64.bin %out_path%\ /y
copy  %source_path%\scp.img %out_path%\ /y
copy  %source_path%\spmfw.img %out_path%\ /y
copy  %source_path%\sspm.img %out_path%\ /y
copy  %source_path%\super.img %out_path%\ /y
copy  %source_path%\vbmeta.img %out_path%\ /y
copy  %source_path%\vbmeta_system.img %out_path%\ /y
copy  %source_path%\vbmeta_vendor.img %out_path%\ /y
copy  %source_path%\tee.img %out_path%\ /y
copy  %source_path%\userdata.img %out_path%\ /y
copy  %source_path%\vendor_boot.img %out_path%\ /y
copy  %source_path%\pi_img.img %out_path%\ /y
copy  %source_path%\dpm.img %out_path%\ /y
copy  %source_path%\mcupm.img %out_path%\ /y
copy  %source_path%\MT6789_Android_scatter.txt %out_path%\ /y
copy  %source_path%\MT6789_Android_scatter.xml %out_path%\ /y
copy  %source_path_sub%\flash.xml %out_path_sub%\ /y
copy  %source_path_sub%\DA_BR.bin %out_path_sub%\ /y
copy  %source_path_sub%\flash.xsd %out_path_sub%\ /y


ECHO.
ECHO. all ����������
ECHO.
pause
goto MENU


:CP_LK_BOOT
CLS
ECHO.
ECHO.               %project_name% һ����������
ECHO.*************************************************************
ECHO.      
ECHO.       ��ʼ���� lk boot ......
ECHO.                     
ECHO.*************************************************************
ECHO.

copy  %source_path%\boot.img %out_path%\ /y
copy  %source_path%\vendor_boot.img %out_path%\ /y
copy  %source_path%\dtbo-verified.img %out_path%\ /y
copy  %source_path%\lk-verified.img %out_path%\ /y
copy  %source_path%\logo-verified.bin %out_path%\ /y
copy  %source_path%\MT6768_Android_scatter.txt %out_path%\ /y
copy  %source_path%\preloader_tb8786p1_64_k510.bin %out_path%\ /y

ECHO.
ECHO. lk boot ����������
ECHO.
::pause
goto MENU

EXIT


