#include "maincpp.h"
#include "Robstrite.h"
#include "main.h"
RobStrite_Motor RobStrite_01(0x01, &hcan1); // 实例化电机对象
uint8_t pRxdata[8], pTxdata[8];
void Configure_Filter(void) ;
void maincpp()
{
    Configure_Filter(); // 配置过滤器
    RobStrite_01.RobStrite_Get_CAN_ID(); //获取设备ID，MCU内部绑定，需要使能
    RobStrite_01.Disenable_Motor(1);
    // RobStrite_01.Set_CAN_ID(0x01);
    // RobStrite_01.Set_ZeroPos();
    // RobStrite_01.Set_ZeroPos(); 
    HAL_Delay(100);
    RobStrite_01.Enable_Motor(); 
    HAL_Delay(100);
    RobStrite_01.Get_RobStrite_Motor_parameter(0x7005); // 获取电机模式
    while (true)
    {
        RobStrite_01.Get_RobStrite_Motor_parameter(0x7005); // 获取电机模式
        float T = 1;          
        float Angle = -1;       
        float Speed = 0.8;       
        float Pacceleration = 1; 
        float Kp = 0.5;          // Kp
        float Kd = 0.5;          // Kd
        // RobStrite_01.RobStrite_Motor_move_control(T, Angle, Speed, Kp, Kd);
        float current = 0.5; // 
        RobStrite_01.RobStrite_Motor_current_control(current);
        // RobStrite_01.RobStrite_Motor_Speed_control(3, 0.1,1);
        HAL_Delay(100);
    }
}
void Configure_Filter(void)
{
    CAN_FilterTypeDef sFilterConfig;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0; // 把接收到的报文放入到FIFO0
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterActivation = ENABLE;
    // sFilterConfig.SlaveStartFilterBank = 14;//为从CAN实例选择启动
    if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) // creat CanFilter
    {

        Error_Handler(); //_Error_Handler(__FILE__, __LINE__);
    }
    if (HAL_CAN_Start(&hcan1) != HAL_OK) // initialize can
    {

        Error_Handler(); //_Error_Handler(__FILE__, __LINE__);
    }
    // 当FIFO0中有消息的时候进入中
    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) // The FIFO0 receive interrupt function was enabled
    {

        Error_Handler(); //_Error_Handler(__FILE__, __LINE__);
    }
}
void HAL_CAN_RxFifo0MsgPendingCallback(
    CAN_HandleTypeDef *hcan) // 中断回调，当我认定的邮箱CAN口对应的邮箱有数据进入，则进入中断
{
    UNUSED(hcan);
    CAN_RxHeaderTypeDef RXHeader;
    uint8_t RxData[8];
    if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RXHeader, RxData) == HAL_OK)
    {
        if (RXHeader.IDE == CAN_ID_EXT)
        {
            RobStrite_01.RobStrite_Motor_Analysis(RxData, RXHeader.ExtId);
        }
    }
}
// .............................................'RW#####EEEEEEEEEEEEEEEEEEEEEEEEWW%%%%%%N%%%%%%NW"...........
// ............................................/W%E$$$$EEEE######EEEEEEEEEEEEEEEE%%@NN@@$@@N%%%%N%]~`........
// ........................................i}}I&XIIYYXF&R#E$$$$$EEE##EEEEEEEEEEEE$N$#$K1:!YW@N%%%%@N$KY]+";..
// .....................................!>>li!"~~~'~~~~~!"i/1lIFK#E$$$EEEEEEE$$EEE%I::.....,]E@@@NNN@M$E$R>..
// ....................................+1"""i>"""""!~''''~~~!!~~!>/]Y&#$$$EEEEWWEEE$F,.......:>IRE$#&I/>'....
// ...................................;*lX&NM@@NW$#RFIl1i"!~~"">>!~~~!i}Y&#$W$EW%$EEMi...........::...'l1....
// ]}/+>~,............................,*YRNNNN@@MMMMMMMM@WRF*1>!~"!~!!~~!>+1IK$W%%W%1.................!*+....
// FFF&K&FYYYI]/"'`....................!K%W$$$$$$$EEEEEEE$W%%%WE&I]+!~~~!">"~~i*#%@#...................';....
// }}}}}}]l*XR#$WWERXl/!,:........,>>i/YK&&&&KKKKRR##EE$$$$$EEEE$$$EKYl/>!'~!"!+]IRNI..................'':...
// lllll]]]]}IYYXFK#W%N%$RFl+~`..`X/>>>!~~~~~!!"""">>ii+/}*YXK#EE$$WWWW$#Fl+"'~+**]*FI"................>i....
// ]]]]]]]]]]YXXXXYYXFRE$WW%%W#FlXl;!">+//i">"~'''''~!""!~~~!""i/1]*YFR#$%%WE&l/1]**lI&!.............>]]ll~..
// ]]]]]]]]]*XXXXXXXXYYX&R$$EE$$WWRR#WWWWW$E##KXI*1>!~!!""""!!!~~~'~~!!"+}I&R$NNWKYll*E"............"}/,~I&'.
// ]]]]]]]]lYXXXXXXXXXXXYYXKE$$E#E$$$$$$$$$$$$$$WWW$#X}1+>>""!''''~!!">""""""/]Y#W%$FRY............./+,.~lF>.
// ]]]]]]]]YXXXXXXXXXXXXXXXYYFKEW$E#EEEEEEEEEEEEEEEE$$WWW$$E#RFYl/+i!''!>"!!~!i]]]*XR#1'............!I/!]XI`.
// ]]]]]]]IXXXXXXXXXXXXXXXXXXYYYFE%WEEEEEEEEEEEEEEEEEEEEEEE$$$%%NN%$EKY]+i"!!"ilII*l]lXK/.:..........;+1/>:..
// ]]]]]]IXYXXXXXXXYYYYXXXXXXXYYR$RK$%$EEEEEEEEEEEEEEEEEEEEEEEE##EE$%NNNWE#R&&XI**llll]Y*.......::`,,`:::....
// ]]]]]*XXXXXXXXXXYYYYYYYXXXY&$#I/>/YE%$EEEEEEEEEEEEEEEEEEEEEEEEEEEE#EE$$WW$$W$ER&Y**]&}~+]IFRE$WW%%%%W$$#KX
// ]]]]lYXXXXXXXYYYYYYYYYYXXYK#I/ii+i>lYKWWEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE$$WW$$#@%NMMM@@NNNN%%NNNNNN@@
// ]]]]YXXXXXXYYYYYYYYYYYXYYKX1iiiii+l1>i}KWWE#EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE$W%@N%%%%%%%%%%%%%%%%%%%
// ]]]*XXXXXXXXXXXXYYYYYXYY&*++iiii+]+>++>11X$%$EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE$%N@@NNNN%%%%%%NNN%%
// ]]]YXXXXXXXYYYYYYXXYXYX&}i+iiiii1+iiii+*>>+*RWWEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE$%N%W$$$$$$$$$WW%%
// ]]*XXYYYYYXX&K#$&YXXXXK}>+iiiii++iiii+FI>+i>>}F$W$EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
// }}IYYXFK#E%N%%NEYXXXYK}>iiiiiiiiii+>1I}]>iiii>"+*R$W$EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
// &XK#EWN@@%#YWN$YYXXY&l>iiiiii++iii>}I"il>iiiiiii"+1IR$$EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
// $NN@N$&}!`.*NWXYXXYFI1/ii++i"!i>+>}l"!i]"iiiiiiii/i>/1IEW$$EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
// "Y&l>,.:~1F@%KYXXXXF}Yi+i"',.';:,1]"+!'/;i">iiii>/i/1"1]IIX#$$$EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
// ......*@M@%RFIYYXYF1F}!'`::::!`."]""~'!]~"":~"iii/i}/+F***>i*YF#E$$EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
// ......`+FWNWERFXYXl+Y`::::::,".!}~,:.:.~',*:::,'!+/}!/]!>l/"}X>i1lXRE$$$EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
// .........;/XEN@N%Wi&].`:::`:!',]/1i~,::`>`}>.`::.~1""1;::~1"+]li">>/llXRE$$$$$EE##EEEEEEEEEEEEEEE####EEEE$
// ............:'+lFK}N+:`:::`:!!&%NW$W$&]~,'!/~.``.~!.!''>+/IY>]lX/""11>ii+1lY&#$WWWW$$$$$E$$$$$$$WWW%%%W$#&
// .................>i~~,`:::`,}#M#}"'F%$W$}`;'!;.:`,','1XFK@@@RF@@@&~~~,!>ii>>+i/}lIXF&KR#$R#RRRKK&XIl1i!'`.
// ................`+'.~'`::`:`}$X`.::"&KFK&,:`'~!:`,'`~!;.;X&FKK$&l##l'::`;!>+/+i>>>>>>ii>I!.:..............
// ...:;..`!/:.....;i;."!`::`:;I+~.:.!E&FW#K':'',!"''';...;lYR#K&#K;"1#]~`::,:>i~i++++iii/}++................
// ...;Y.;/Y`......'>;.>+,::`.~E]:::.'K/"l}i`:`:::`;;'!;`.iK/}%&lRI.`1*'``:,,:+"~,,~"i++i1#+},...............
// ...:,>/.......~"'.;l;:``:'Y]'```./+~'';::``::::::`,`:~/~~i"!/'.:i;:`:,,:/+;''.::,'!"**>li...............
// ....:,.;;.......~"':.>i:`:',+l>;'';'";;,``:`:::::::::`,`'~~~~+":`~~```;;'>li!;`::::,.`Xi.1l`..............
// .......'];......`i;;.:I'::'I>1>'~~'';;,,```::::::::```,;;''~~''~~'`,'>>>>'/"~`,::::,`,X".`l+..............
// ........~~......."'~`/>+:::lll";'';;;,,,``:``;~::::```,;;'''~!>>"!!>ii!;:,~,'+!::::`;,Y~..,Y'.....::......
// .......:,.........!~/!.~+`.>]*>:,,,,,,````:`;;;`::````,,,,;;;~!'1/"!;`:::!~+]+"`::`:;;*~...~l.....`>,.....
// ........>..........>i!!++!'`/1Ii`::```````:::::`:::::`````,,,,`'i``:::::;]}/iii,:`::`!*~....>".....`/`....
// ........,`........;>.'>"~.i++]/ll+'`::::::``````::::::````::..,+```:`:`:"+ii+++':`:`:;Fi.....i`.....;+....
// ........`,.......`+:~!.;':i++/+i/}}1+>~;`::.....::::::...:,~i]X~`,:`:`:;/+++++1!:`::`.]I.....`"......+;...
// ........:~......:i,~'..";,ii+/+++++/1}]]}1/i>"!!;,,,,,>}lII**Y>`,:`:;,,/++++/+1+`:::`:,F,.....~,.....'>...
// ................"~'`..:1,,+i1+++//////}111}}}IY$K">>>!*NFl&X]>,,:`.'~`*]++++//+}':`:``.>1.....:".....'i...
// ...............~i':...!1.'+//+++//+/+]l+1]lIF]/Kl"">>+11>"1&i``::`!~;]I]++++//i1i:`::~;./;.....",....!;...
// ..............,1;.....]~.~+}++++/+///*]Y&F$Kl+}1!i++}1+i"11'`:::;!~i*]l]+//+/1++/,:`:~i``/.....'".........
// .............:]~....."}.:"}/+++////i1XRRYF*]lFKY/lI/`;,:"+~!:::,!"l&XYFY++/+/]++/~:`:~+".~i....;/.........
// .............+>.....`*;::/}++++//+/*#El}FIl*F&X]I*IX+`;1}'i::,~>+FYIX&#%#/++1]i++>:`:~++;.+'...~l.........
// ............;}......+!.:'l+++++1+/#N@/'i#F1!1]*"l*I*]+"+l}1+i+i>iRRE$$$ENIi+1]++//;::!++":`i.:.1].........
// ............/~....."i.`.+1i+++/1i*WW&~!1Wi`,+i/]Il>'`:.'I*Y>!>}FE$$EEEE#%*i+}}++1}!:`>+i+,.!~."Ii.........
// ............]'....'/.`;`1+++++//i&EW*~~YF'>+}]1//i"`.:''.Y+iY#$$EEEEEEEEW]i+}/++/*i`,++i/]::>+*l,.........
// ...........`l"...`/`:"~'/}/+++//YWEW*!+*+>iiI]]/">>i;,!`.lX$$EEEEEEEEEEEWN*+1++++Y1;'+i+i*+.~l}~..........
// ...........`l/,..+;.,+'~]*+//+1EW$E$X+1"""iY&i1l>"""ii+`,EWEEEEEEEEEEEEEE$W1++++i*]'>+i++//~.+;...........
// ............]}/`"".:!+~!I}+//iY%#$$EX1~""iFI*~i1]"">!/]`"%EEEEEEEEEEEEEEE$$1i++++/1"+++++};".!;...........
// ............'Y/1+.`:>+>/]1+//iXW#E$$**X>+F*/";">]l!"+&%I+WE$$$$$$$E$$WW%N%}i++++/>i+i++++]:;;,>...........
// .............~l}`::;i+i]i}+/+l@%$$E$&XYYY}!1,.!i>XIYX#N$REWEEEEEEEEEEEEE$N1+/+++/'iiiiii/1.`>:+...........
// ..............,i::;}i+i],*+++$@$EE$E%&']!~"+;~;>!"*}>$$$$EEE$EEEEEEEEEEE#%Fi//+1~'/i+++il>..>`i`..........
// ..............';:./]i++}.}li]NEEEE$W$}:i]+i!;~;i>i>,>%F/*$EEWWEEEEEEEEEEEE%Xi+//./+i++++*`..";i`..........