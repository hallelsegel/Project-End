   M S B A M L   `   `   `  ����  	WPFclient    WPFclient.game   5   	   \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     6F     � Icon$  icon.ico��5   	    � Title$
 game��5   	   $	��700��6   $	��416��6#    � 
ResizeMode$ CanMinimize=�6/   .��5      � .+�5   
   �� $��
Trivia Pro��6   $��40(�6%   $��Center=�63   $��0,50,0,0q�6P   $	��Top=�6b   $
��Bold"�6z   �� 5	   
   $��roomName��6   $
��Auto��6#   $
��Auto��60   $��20(�6>   $��Center=�6L   $��Center=�6p   $��Center=�6�   $��	0,0,0,425q�6�   �� 5
   
   -   5        $��
currQLabel��5
      $��question: 1/5��6"   $
��Auto��6:   $
��Auto��6G   $��17(�6U   $��Center=�6c   $��Center=�6�   $��Center=�6�   $��	0,0,0,360q�6�   �� 5   
   -   5        $��questionLabel��5       �� TextAlignment$ Center=�6'   $��0q�6>   $��20(�6R   $
��Left=�6`   $��64��6{   $��
21,197,0,0q�6�    �� TextWrapping$
 Wrap=�6�   $��TextBox��6�   $	��Top=�6�   $	��368��6�   �� 5   
   -   5         ��Name$ button1��5      $	��368��6"   $��46��6.   $��	10,0,10,0q�6:   $��17(�6N   
����İ�$��answer 1��6x   $��Center=�6�   $��Center=�6�   $��21,288,21,337q�6�   6�   �� 5   
   -   5        $ button2��5      $	��368��6"   $��46��6.   $��	10,0,10,0q�6:   $��17(�6N   
����İ�$��answer 2��6x   $��Center=�6�   $��Center=�6�   $��21,348,21,277q�6�   6�   �� 5   
   -   5        $ button3��5      $	��368��6"   $��46��6.   $��	10,0,10,0q�6:   $��17(�6N   
����İ�$��answer 3��6x   $��Center=�6�   $��Center=�6�   $��21,408,21,217q�6�   6�   �� 5   
   -   5        $ button4��5      $	��368��6"   $��46��6.   $��	10,0,10,0q�6:   $��17(�6N   
����İ�$��answer 4��6x   $��Center=�6�   $��Center=�6�   $��21,468,21,157q�6�   6�   �� 5   
   -   5         ��Name$ 
timerLabel��5      $��5��6$   $
��Auto��60   $
��Auto��6=   $��20(�6K   $
��Bold"�6Y   $��Center=�6k   $��Center=�6�   $��Center=�6�   $��	0,460,0,0q�6�   �� 5   
   -   5        $��score��5      $��score: ��6   $
��Auto��6/   $
��Auto��6<   $��25(�6J   $
��Bold"�6X   $��Center=�6j   $��Center=�6�   $��Center=�6�   $��	0,550,0,0q�6�   �� 5   
   -	   5        $ UserName��5      $��UserName��6"   $
��Auto��65   $
��Auto��6B   $��20(�6P   $��Center=�6^   $
��Left=�6�   $	��top=�6�   $��	0,0,0,425q�6�   �� 5   
   -
   5        $��50��5      $��45��6   $��0,0,0,0q�6(   $��31(�6:   
����İ�$
��left=�6d   $��Bottom=�6   $��	20,0,0,20q�6�   .��5      �� $��Resources\HomeIcon.png��6   $��20��64   $��20��6?   5      �� 5   
   -   5         ��Name$ imageCorrect��5      $
��Left=�6&   $	��700��6A   $��Resources/BG_green.png��6O   $	��Top=�6o   $	��416��6�   $y�Hidden=�6�    J� Opacity$ 0Q�6�   �� 5   
   -   5        $ imageIncorrect��5      $
��Left=�6(   $	��700��6C   $��Resources/BG_red.png��6Q   $	��Top=�6o   $	��416��6�   $y�Hidden=�6�   $ 0Q�6�   �� 5   
   -   5        $ textCorrect��5      $��CORRECT!��6%   $��40(�68   $��Center=�6F   
���/���$��0,0,0,0q�6|   $��Center=�6�   $
��Bold"�6�   $y�Hidden=�6�   $ 0Q�6�   �� 5   
   -   5        $ textIncorrect��5      $��
INCORRECT!��6'   $��40(�6<   $��Center=�6J   
�������$��0,0,0,0q�6   $��Center=�6�   $
��Bold"�6�   $y�Hidden=�6�   $ 0Q�6�   5      5      