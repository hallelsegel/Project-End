﻿#pragma checksum "..\..\addQuestion.xaml" "{406ea660-64cf-4c82-b6f0-42d48172a799}" "F606B793BAD149B8554FAD4A4185BDE2"
//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.42000
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Automation;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Markup;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Effects;
using System.Windows.Media.Imaging;
using System.Windows.Media.Media3D;
using System.Windows.Media.TextFormatting;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Shell;


namespace WPFclient {
    
    
    /// <summary>
    /// addQuestion
    /// </summary>
    public partial class addQuestion : System.Windows.Window, System.Windows.Markup.IComponentConnector {
        
        
        #line 14 "..\..\addQuestion.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.TextBox Question;
        
        #line default
        #line hidden
        
        
        #line 15 "..\..\addQuestion.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.TextBox Answer1;
        
        #line default
        #line hidden
        
        
        #line 16 "..\..\addQuestion.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.TextBox Answer2;
        
        #line default
        #line hidden
        
        
        #line 17 "..\..\addQuestion.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.TextBox Answer3;
        
        #line default
        #line hidden
        
        
        #line 18 "..\..\addQuestion.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.TextBox Answer4;
        
        #line default
        #line hidden
        
        
        #line 19 "..\..\addQuestion.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button Submit;
        
        #line default
        #line hidden
        
        private bool _contentLoaded;
        
        /// <summary>
        /// InitializeComponent
        /// </summary>
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "4.0.0.0")]
        public void InitializeComponent() {
            if (_contentLoaded) {
                return;
            }
            _contentLoaded = true;
            System.Uri resourceLocater = new System.Uri("/WPFclient;component/addquestion.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\addQuestion.xaml"
            System.Windows.Application.LoadComponent(this, resourceLocater);
            
            #line default
            #line hidden
        }
        
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "4.0.0.0")]
        [System.ComponentModel.EditorBrowsableAttribute(System.ComponentModel.EditorBrowsableState.Never)]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Design", "CA1033:InterfaceMethodsShouldBeCallableByChildTypes")]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Maintainability", "CA1502:AvoidExcessiveComplexity")]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1800:DoNotCastUnnecessarily")]
        void System.Windows.Markup.IComponentConnector.Connect(int connectionId, object target) {
            switch (connectionId)
            {
            case 1:
            this.Question = ((System.Windows.Controls.TextBox)(target));
            
            #line 14 "..\..\addQuestion.xaml"
            this.Question.GotFocus += new System.Windows.RoutedEventHandler(this.TextBox_GotFocus);
            
            #line default
            #line hidden
            
            #line 14 "..\..\addQuestion.xaml"
            this.Question.LostFocus += new System.Windows.RoutedEventHandler(this.TextBox_LostFocus_Question);
            
            #line default
            #line hidden
            return;
            case 2:
            this.Answer1 = ((System.Windows.Controls.TextBox)(target));
            
            #line 15 "..\..\addQuestion.xaml"
            this.Answer1.GotFocus += new System.Windows.RoutedEventHandler(this.TextBox_GotFocus);
            
            #line default
            #line hidden
            
            #line 15 "..\..\addQuestion.xaml"
            this.Answer1.LostFocus += new System.Windows.RoutedEventHandler(this.TextBox_LostFocus_Answer1);
            
            #line default
            #line hidden
            return;
            case 3:
            this.Answer2 = ((System.Windows.Controls.TextBox)(target));
            
            #line 16 "..\..\addQuestion.xaml"
            this.Answer2.GotFocus += new System.Windows.RoutedEventHandler(this.TextBox_GotFocus);
            
            #line default
            #line hidden
            
            #line 16 "..\..\addQuestion.xaml"
            this.Answer2.LostFocus += new System.Windows.RoutedEventHandler(this.TextBox_LostFocus_Answer2);
            
            #line default
            #line hidden
            return;
            case 4:
            this.Answer3 = ((System.Windows.Controls.TextBox)(target));
            
            #line 17 "..\..\addQuestion.xaml"
            this.Answer3.GotFocus += new System.Windows.RoutedEventHandler(this.TextBox_GotFocus);
            
            #line default
            #line hidden
            
            #line 17 "..\..\addQuestion.xaml"
            this.Answer3.LostFocus += new System.Windows.RoutedEventHandler(this.TextBox_LostFocus_Answer3);
            
            #line default
            #line hidden
            return;
            case 5:
            this.Answer4 = ((System.Windows.Controls.TextBox)(target));
            
            #line 18 "..\..\addQuestion.xaml"
            this.Answer4.GotFocus += new System.Windows.RoutedEventHandler(this.TextBox_GotFocus);
            
            #line default
            #line hidden
            
            #line 18 "..\..\addQuestion.xaml"
            this.Answer4.LostFocus += new System.Windows.RoutedEventHandler(this.TextBox_LostFocus_Answer4);
            
            #line default
            #line hidden
            return;
            case 6:
            this.Submit = ((System.Windows.Controls.Button)(target));
            
            #line 19 "..\..\addQuestion.xaml"
            this.Submit.Click += new System.Windows.RoutedEventHandler(this.click_submitQuestion);
            
            #line default
            #line hidden
            return;
            case 7:
            
            #line 20 "..\..\addQuestion.xaml"
            ((System.Windows.Controls.Button)(target)).Click += new System.Windows.RoutedEventHandler(this.click_mainMenu);
            
            #line default
            #line hidden
            return;
            }
            this._contentLoaded = true;
        }
    }
}

