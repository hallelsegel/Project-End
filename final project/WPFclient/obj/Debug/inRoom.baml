   M S B A M L   `   `   `  ����  	WPFclient    WPFclient.inRoom   5   	   \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     6F     � Icon$  icon.ico��5   	    � Title$ inRoom��5   	   $	��700��6   $	��416��6%    � 
ResizeMode$ CanMinimize=�61   .��5      � .+�5   
   �� -   5         ��Name$ UserName��5      $��UserName��6"   $
��Auto��65   $
��Auto��6B   $��20(�6P   $��Center=�6^   $
��Left=�6�   $	��top=�6�   $��	0,0,0,425q�6�   �� 5	   
   $��
Trivia Pro��6   $��40(�6%   $��Center=�63   $��0,50,0,0q�6P   $	��Top=�6b   $
��Bold"�6z   �� 5
   
   -   5        $
 room��5
      $��room name: ��6   $
��Auto��64   $
��Auto��6A   $��31(�6O   $��Center=�6]   $��Center=�6�   $��Center=�6�   $��	0,0,0,400q�6�   �� 5   
   $��Connected Users��6   $
��Auto��6*   $
��Auto��67   $��25(�6E   $��Center=�6S   $��Center=�6w   $��Center=�6�   $��0,0,0,90q�6�   $
��Bold"�6�   �� 5   
   -   5         ��Name$ 	usersList��5      $��Center=�6%   $	��300��6B   $	��200��6N   $��	0,150,0,0q�6[   $��23(�6n   $��Center=�6|   $��Center=�6�   �� 5   
   -   5        $ questionNum��5      $��Number of questions - ��6%   $
��Auto��6F   $
��Auto��6S   $��20(�6a   $��Center=�6o   $��Center=�6�   $��Center=�6�   $y�Visible=�6�   $��	0,0,0,300q�6�   �� 5   
   -   5        $
 time��5      $��Time per question - ��6   $
��Auto��6=   $
��Auto��6J   $��20(�6X   $��Center=�6f   $��Center=�6�   $��Center=�6�   $y�Visible=�6�   $��	0,0,0,250q�6�   �� 5   
   -   5        $ players��5      $��Max number of players - ��6!   $
��Auto��6D   $
��Auto��6Q   $��20(�6_   $��Center=�6m   $��Center=�6�   $��Center=�6�   $y�Visible=�6�   $��	0,0,0,200q�6�   �� 5   
   -   5         ��Name$ adminStartButton��5      $	��200��6+   $��45��67   $��	10,0,10,0q�6C   $��31(�6W   
����İ�$y�Visible=�6�   $��
Start Game��6�   $��Center=�6�   $��Center=�6�   $��	0,460,0,0q�6�   6�   �� 5   
   -   5        $ adminExitButton��5      $	��200��6*   $��45��66   $��	10,0,10,0q�6B   $��31(�6V   
����İ�$y�Visible=�6�   $��
Close Game��6�   $��Center=�6�   $��Center=�6�   $��	0,585,0,0q�6�   6�   �� 5   
   -	   5        $ 
joinerExit��5      $	��200��6%   $��45��61   $��	10,0,10,0q�6=   $��31(�6Q   
����İ�$y�Hidden=�6{   $��
Leave Game��6�   $��Center=�6�   $��Center=�6�   $��	0,520,0,0q�6�   6�   ��5   
   �� 5       �� ImageSource$ BG.png��6   5      5      