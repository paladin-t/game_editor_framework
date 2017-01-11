/**
 * @version: 1.1
 * @author: Wang Renxin,
 *     https://github.com/paladin-t/game_editor_framework
 *     mailto:hellotony521@qq.com
 * @file: This file is a part of GEF, for copyright detail
 *     information, see the LICENSE file.
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.ComponentModel;

namespace gef
{
    internal static class ShellUtil
    {
        private static Stack<string> workingDirStack = null;

        public static string GetWorkingDirectory()
        {
            return Environment.CurrentDirectory;
        }

        public static void PushWorkingDirectory()
        {
            if (workingDirStack == null) workingDirStack = new Stack<string>();
            workingDirStack.Push(Environment.CurrentDirectory);
        }

        public static void PushWorkingDirectory(string n)
        {
            PushWorkingDirectory();
            Environment.CurrentDirectory = n;
        }

        public static string PopWorkingDirectory()
        {
            if (workingDirStack == null) workingDirStack = new Stack<string>();
            if (workingDirStack.Count == 0) throw new Exception("Working directory stack empty");
            string curr = workingDirStack.Pop();
            Environment.CurrentDirectory = curr;
            return curr;
        }

        public static T Parse<T>(string text)
        {
            return (T)TypeDescriptor.GetConverter(typeof(T)).ConvertFromInvariantString(text);
        }

        public static string ToString<T>(T obj)
        {
            return TypeDescriptor.GetConverter(typeof(T)).ConvertToString(obj);
        }

        public static IntPtr GetPtr(Control c)
        {
            return c.Handle;
        }

        public static string GetExceptionMsg(Exception ex)
        {
            string msg = string.Empty;
            if (ex.Message != null) msg += "Info: " + ex.Message;
            if (ex.Message != null && ex.InnerException != null) msg += "\n";
            if (ex.InnerException != null) msg += "Inner: " + ex.InnerException.Message;

            return msg;
        }

        public static DialogResult MsgBox(string text, MessageBoxButtons btn, MessageBoxIcon icon)
        {
            return FormMain.GetInstance().MsgBox(text, btn, icon);
        }

        public static DockStyle ConvertDockStyle(string style)
        {
            switch (style.ToLower())
            {
                case "none":
                    return DockStyle.None;
                case "top":
                    return DockStyle.Top;
                case "bottom":
                    return DockStyle.Bottom;
                case "left":
                    return DockStyle.Left;
                case "right":
                    return DockStyle.Right;
                case "fill":
                    return DockStyle.Fill;
            }

            return DockStyle.None;
        }
    }
}
