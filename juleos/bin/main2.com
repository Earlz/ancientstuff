� main.c<�    Turbo Assembler  Version 4.1�� @�L�  h� @��� _TEXTCODE�� (Dܖ _DATADATA Hp��
 _BSSBSS*� H�d� DGROUP�� ��U�r SPUSH@ SCOPY@ LMOD@ LDIV@ 
_biosequip _bioskey 	_biosdisk 	_biostime _strtol _peekb _pokeb _atol _itoa @�  
__scrollup �  _nreadB ��  	_FDFS_newo
 ��  	_load2mem� 4�  _set� �  _nhelp
 ϐ  _setvideomode�  [�  _iseven� ϐ  _clsA z� _DISK_STATUS_ERROR� �  _write_entry� e� _Nsect  ��  _trapme) B�  _Shelp! А  _Load_lowbasic� �  _notesN z�  	_int_inito (�  _nset� J�  _memsizeH  ��  _DrawBox� A�  	_num2asciQ r�  _putcur� ��  _sfdfs� �  _box� �  _sectadr� �  _testing� 1�  _printequip. b� _total_memory� +� _VideoMemory� ݐ  _getkey� ��  	_checkmem� ��  	_FDFS_del�
 ��  	_int2str2s x�  _sloadz ^�  _shtimeJ �  _bprint�  ��  _getvideomode�  [�  _switch2flat ~�  _main* �  _inputn Q�  _int22� �� _shell_v� i�  _nextseg� u� _indexb	  u�  _farjmp� ڐ  _sformat
 �  _init� W�  _scanf> ��  _getmem� �� _Ndrive  ��  _Scheck_scroll� �
  _nl2 �  _nwrite� ��  _printai ��  _printf I�  _printn� ��  _SMain� �  _word2bytes$  ��  _bprint2�  O� _con_inf� �� _command	 
�  _background� 
�  _Sview� �  _reboot�  ��  _pokeb32S .� _gindex� �  _poke/ �  _verV L�  _getcury�  �  _getcurx� !�  	_absreadF 
 ��  	_absreadN�	 ��  _input� �  
_FDFS_load�
 !�  
_abswriteN�	 �� _video_page  ݐ  _isset� ��  _FDFS_format�
 *� _DISPLAY_TYPE   6�  _setvideopage�  ;�  _docmd# Ő  
_byte2word   S�  _putdotg  ��  _test2 ސ  _test4� @�  _flush_indexv א  _strlenH �  _write_diskN/ 7�  _write_diskF� ��  _write_disk2� ��  _streqc h�  _read_diskF` ��  _read_diskN� ��  _read_disk2\ �� @��3main.c6� @��3prepu.c�� @� �I:\TC\INCLUDE\bios.h�� @� �I:\TC\INCLUDE\stdlib.h� @� �I:\TC\INCLUDE\math.h�� @�&��2include\conv.hÈ @�t3
lowlevel.ci� @��3
midlevel.c�� @�d�3include\stdio.h׈ @�Q�3FDFS\FDFS.c�� @鱅�2FDFS\init.c
� @���3FDFS\raw_RW.hӈ @�ܤ3FDFS\oper.c�� @�ݤ3FDFS\utils.c)� @麭3FDFS\index.cU� @�t3shell\main.c�� @�q�
3shell\interf.cF� @���3shell\notes.c� @�L��  U����F�F��F�F��F��f��F��F�� ��]�U����F�f��F��F��F��F��F��F�� ��]�U�������F��F� �F��F�� ��]�U��V�F�@���v�F� PV� �P�  ��^]�U�� �F�]�U������F��F��F��F�� ��]�U��F��F� ]�f�������f�    �  �  e�$�U���F�]�U��V�F� �� ����F� ���V�Y<u'���F� PV� �P�  ��� P��@P� �P�  ���'�F� P��@P� �P�  ��� P��@@P� �P�  ��^]�U���`��F�n�N�v
�V�F�^��f�a�F�� ��]�U���F�n�N�v
�V�F�^�]�U���v�V�> �]�U���� ��f��F��F�� ��]�U�����> ��V��F��F��F�� ��]�U�����> ��v��F��F��F�� ��]�U���F�N�n�V
�v� �]�U����^��F�`�v��^�ۋ^X�a��]�U��VVPf�v��f�    �F�X^^]��  �û�&�&��� ��رA�_�^��ø P�|Y�U���v�v]&�f ]�U��VW3��~��� P�F�P�v�  ��GF;v
v�_^]�U��� V��v�v�  YY�� �F;vv늆 �� ^��]���     ��   �� f3�f3���f��f.
.�  ��"����$�"����U��V3��F�F�^�? u��� ^]�U��VW�~W���YP�v���YZ;�u%��3���ފ�^: t��� �FW��Y;�w��� � _^]�U���VW�F�F��v
�F� �,� ��vV�F��P����;~t�F
F��M ��Ŷ������+�A�T���(T�.T�vTƗ����
�
Tf���;�u���F�  F� �F��u�_^��]�U��VW�v��~��vWV�R���G�F
+F;�r�F�F+F;�r�_^]������P� P�k�YY�PSQW�@ �ش� �� � �� �5�J ����� � � ��_Y[X�U����F�
 ��]�U���V�ˉ^��v������� ^��]�U���V�v���v���F� ��F��F�� ^��]��U���VW�v�F��F� ���#����� P�  Y�~���� P�  Y�t����F��F���< u���F� ��F��F�� � �8.9t	��CC��� .�g  K � H���;�u����P���P�N�YYN� �F��(��P�g��P�5�YY� �y�P�U�P� P�b���N�F� �1� �F� �F��$� �F� �F���G�P�#��P���YYNN�F�� �~� u��� N�$�P� ��P���YY��P���P�4�������P�����P��YYF�F� �~� t������#���� _^��]�U���V�v�F� �6�<u�������P��P�l�YY��P��P�4������P����P�M�YYF�< u�^��]�U����c��F��}��F��v�v�%�YY�v��Y�v��v���YY��]�U����
 P�F�P�v�  ���F�P�_�Y��]�U���V�v���Y� P�F�P�v�  ���Q��F�P�1�Y�v��C��<u������ � ^��]�U���2VW�F�P��Y�v�V�  Y����� _^��]�U��~;u
���
 P���Y]�U��� VW� 3�� �j��F��F���~���~�� P�w�Y��~� u����H���}�P�Y��P�'�YY�F�� = t��= t@��=� tB���G�u���: P�0�Y� 3�P�&�Y�:�P���PG�3 ���BT�^T�`T�bT�dT�����8�S�������YYƂ~� �������Ƃ~� 3��	Ƃ~� 3�� F��u���]���~�� _^��]�U����v�F� P�v�v
�v�v� P�  ���F��F�� ��]�U����v�F� P�v�v
�v�v� P�  ���F��F�� ��]�U����F�F��F�F��F�F��F
�F��F�F�`�F���n��N��v��V��^�Ë^��f�a�F�� ��]�U����F�F��F�F��F�F��F
�F��F�F�`�F���n��N��v��V��^�Ë^��f�a�F�� ��]�U���
V�v�F�  �F�  �F�  �ƻ�3���F��F������+Ћ�ƻ 3���F��F�� ���+Ћ�v��~� u���F� �~�Pu���F�  �F� �  S�^�S�	 �  �  � ^��]�U���
�^�S�v�e�YP�	 �  �F� �F��F�F��v
�^��ҋù
 �  �j����F��F�� ��]�U���
�^�S�v��YP�	 �  �F� �F��F�F��v
�^��ҋù
 �  ������F��F�� ��]�U���
�^�S�v���YP�	 �  �F� �F��F�F��v�v
�^��ҋù
 �  � ����F��F�� ��]�U���
�F� �F��F�  �F�  �F� ǆ�� ����t
��� P�v�Y��]�U��]�U��]�U����F� �F��F�  �F�  �F� Ɔ�� �~�Pu���F�  �F��~�u���=����P� P� P�v��v��v�� P�  ���F��~� v��� P���Y�F��K�F��Ɔ�� Ɔ��0Ɔ��9Ɔ��Ɔ��Ɔ��Ɔ �Ɔ� �F� �~� v���* P��Y�F��� � ��]�U���V�F�	���F��F� ��ފ�	 �� ��� v鍆 �PǜQ ŬŽ�\���������+�J�w�����H�YǠ������ P�P�v�������������� ^��]�U���VW�~�Ǳ�����	 ��^���FF�F��~�v� P�v�|�YY�F��F�� _^��]�U����9 P��Y���V P���Y���� P���Y�
 P�F�P��P�  ���F�� P�i�Y��]�U����� P��Y����� P��Y����P��Y���F�P�J�Y�#P�F�P���YY<u	���w �F��(P�F�P���YY<u	��� �F��-P�F�P��YY<u	���� �F��3P�F�P��YY<u	����F��7P�F�P�s�YY<u
��3��
�F���o���]ø<P��Y���WP���Y���tP���Y����P���Y�����P���Y����U���
�^�S�Z P��YP�	 �  �F�  �F���P��Y������P� � P�P�6 �L�������P��Y��]�U���
�^�S�Z P�S�YP�	 �  �F�  �F���P�T�Y�i�����P���Y����P� � P�P�6 ������]�U����P��Y���F��F�� �'��+P��Y����F��F�� ��]�U����F� �MP�	P�+�YY<u	�����F��RP�	P��YY<u	�����F��VP�	P���YY<u	������F��ZP�	P���YY<u	���3��F��aP�	P���YY<u	�����F��gP�	P��YY<u	�����F��lP�	P��YY<u	���k�F��qP�	P�u�YY<u	���"�F��wP�	P�[�YY<u	������F��~P�	P�A�YY<u	���!��F���P�	P�'�YY<u	���3�F���P�	P��YY<u	����F���P�	P���YY<u	����F��~� u
����|������3�k�vā��	įĺ��������&�@�e�p�{ņő�����2�<�`�w���O�fƈƌƢƦƼ�����������
��$�(�>�B�X�\�r�vǌǐǦǪ��������P��Y�F� ��]�U���"�F� �F� �F� �F�P�5 Y�F���P�]�Y��p���P�P�Y�	P���Y�N��� ��F�
�uދ�]�U��V�v�P��Y�<u	���P��Y�<~�������t��Y�P�r�Y��^]ø�P���Y���P���Y���)P���Y���FP���Y���]P���Y���P��Y���P��Y����P��Y��P��Y��[P��Y��P��Yø�P�z�Y���P�o�Y��F�<v������U���VW�F��F� �F� 3��2��F��`� P�  Y�~���\�P�v��v��t����F��~�Pu	���F� �F��~�u���F� �F� 3��&��F��'v�3�G;~�~�3���3�� P�  Y�t�_^��]�U��� VW3�PP3�P�  ���V�F�3Ҹ RP�v��v��  �V�F�3Ҹ< RP�v��v��  �F�3Ҹ< RP�v��v��  �F��
 P�F� P�v��  ����
 P�F� P�v��  ����W�G�Y�\�V�?�Y�T�_^��]�U����
 P�F�P�} P�  ���F��~�1u
���P��Y�F�P��Y��]øh P� � �RP�  ���U���V�  �� P�F�PV�  ���F�^��]�U��]ø!P��Y�1P��Y���7P��Y�HP��Y�U����F� �F�  �F�  �F�  �NP��Y��xP�t�Y��P�i�Y�~�`���2�� � �  �û  �a��P�D�Y�Y�  �û  &�'��]�U��� �F�h �F�i ��]��V�G���� s����P��Y��<P���Y����6�^�U�������9�3�P�[�Y�F���]�М� �,�9�AĘģĮĹ����������������S�_���������(�=�m�~���������������
�� �EǂǍǜѠ�    Phello hmm error level 1 error @format error @writing still in alpha testing stage This interface for format will change a lot a bit later Type the drive number:  welcome to Notes beta This is quite unstable but demonstrates index's and is more of a diagnostic utility type help for list help read write set exit read -reads an index entry write -writes an index entry help -displays this new -creates a new index, not working exit -exits Notes This is what was stored on the floppy type what you want stored on the floppy  drive number(default is 0:  abssector number(default is 80):  help ver cls reboot basic test time test2 format notes load fdfs view Command not recognized! JuleOS version 0.0.4 pre-alpha ->: basic -Starts the basic interpreter -not included in this version help -Displays this text reboot -Reboots the PC -beta cls -clears the screen format -formats a floppy disk using FDFS -beta time -displays current time -alpha ver -displays version information test -a reserved te����sting command to prevent clutter -use at own risk test2 -another test function so you can work on 2 things at one time load -loads up to 18 sectors(4kb) from a floppy and jumps to it notes -loads up the Notes 0.0.3 diagnostic program(use at own risk) Currently under development. Remeber this is an alpha! JuleOS yay Shell version:  0.0.3 JuleOS version:  0.0.4 this will load sectors from a floppy disk the interface is poor so it will load 18 sectors from cylinder 50 so here we go, this can take a few seconds done loading, now jumping Your system does not meet the memory requirements for JuleOS you need at least 4mb of total memory to run JuleOS ��  t