@ECHO OFF

set project_name=tb8786p1_64_k510

set "source_path=%~dp0out\target\product\%project_name%\merged"
set out_path=\\192.168.0.246\�����\��ʱ�ļ�\zhangxiangfei\out\t8633\t8633_Latest

mkdir %out_path%

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
copy  %source_path%\md1img.img %out_path%\ /y
copy  %source_path%\MT6768_Android_scatter.txt %out_path%\ /y
copy  %source_path%\preloader_tb8786p1_64_k510.bin %out_path%\ /y
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


