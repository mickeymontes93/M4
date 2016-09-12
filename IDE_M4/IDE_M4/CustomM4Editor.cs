using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using FastColoredTextBoxNS;

namespace IDE_M4 {
    public partial class CustomM4Editor : UserControl {
        private string fileName;
        public readonly Style BlueStyle = new TextStyle(Brushes.Blue, null, FontStyle.Bold);
        public readonly Style GreenStyle = new TextStyle(Brushes.Green, null, FontStyle.Regular);
        public readonly Style RedStyle = new TextStyle(Brushes.DarkRed, null, FontStyle.Regular);

        private Regex StringRegex =
                 new Regex(
                     @"
                            # Character definitions:
                            '
                            (?> # disable backtracking
                              (?:
                                \\[^\r\n]|    # escaped meta char
                                [^'\r\n]      # any character except '
                              )*
                            )
                            '?
                            |
                            # Normal string & verbatim strings definitions:
                            (?<verbatimIdentifier>@)?         # this group matches if it is an verbatim string
                            ""
                            (?> # disable backtracking
                              (?:
                                # match and consume an escaped character including escaped double quote ("") char
                                (?(verbatimIdentifier)        # if it is a verbatim string ...
                                  """"|                         #   then: only match an escaped double quote ("") char
                                  \\.                         #   else: match an escaped sequence
                                )
                                | # OR
            
                                # match any char except double quote char ("")
                                [^""]
                              )*
                            )
                            ""
                        ",
                     RegexOptions.ExplicitCapture | RegexOptions.Singleline | RegexOptions.IgnorePatternWhitespace |
                     RegexOptions.Compiled
                     ); //thanks to rittergig for this regex

        private Regex CommentRegex = new Regex(@"#.*$", RegexOptions.Multiline | RegexOptions.Compiled);
        private Regex NumberRegex = new Regex(@"\b\d+[\.]?\d*([eE]\-?\d+)?[lLdDfF]?\b|\b0x[a-fA-F\d]+\b",
                                      RegexOptions.Compiled);
        private Regex AttributeRegex = new Regex(@"^\s*(?<range>\[.+?\])\s*$", RegexOptions.Multiline | RegexOptions.Compiled);
        private Regex KeywordRegex =
            new Regex(
                @"\b(var.int|var.float|var.char|var.str|var.bool|if|else|switch|case|default|break|while|do|for|console.write|console.read|func.int|func.float|func.char|func.str|func.bool|func.void|arr.int|arr.float|arr.char|arr.str|arr.bool|MAIN|var.file|var.fopen|var.fscanf|file.fgets|file.fclose|split|trim|round|sqrt|substring|pow|length|concat|replace|equal|sin|cos|tan|arcsin|arccos|arctan|log|true|false|cadena)\b",
                RegexOptions.Compiled);

        public string FileName {
            get { return fileName; }
            set { fileName = value; }
        }

        public CustomM4Editor(string fileName) {
            InitializeComponent();
            this.fileName = fileName;
            try {
                abrir();
            } catch (Exception e) {
                guardar();
                abrir();
            }
        }

        public void deshacer() {
            this.fctb.Undo();
        }

        public void rehacer() {
            this.fctb.Redo();
        }

        public void copiar() {
            this.fctb.Copy();
        }

        public void pegar() {
            this.fctb.Paste();
        }

        public void cortar() {
            this.fctb.Cut();
        }

        public void seleccionarTodo() {
            this.fctb.SelectAll();
        }

        public void guardar() {
            this.fctb.SaveToFile(fileName, Encoding.UTF8);
        }

        public void abrir() {
            this.fctb.OpenFile(fileName);
        }

        private void fctb_TextChanged(object sender, TextChangedEventArgs e) {
            //clear previous highlighting
            e.ChangedRange.ClearStyle(BlueStyle);
            //highlight tags
            e.ChangedRange.SetStyle(BlueStyle, KeywordRegex);
            e.ChangedRange.SetStyle(RedStyle, StringRegex);
            e.ChangedRange.SetStyle(GreenStyle, CommentRegex);
        }
    }
}
