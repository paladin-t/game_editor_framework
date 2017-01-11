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
using System.Windows.Forms;

namespace gef
{
    public static class Program
    {
        public static void Run(string[] args)
        {
            FormSplash.GetInstance().Show();
            FormSplash.GetInstance().Status = "Starting up";
#if DEBUG
            if (args.Length == 0)
                Application.Run(new FormMain());
            else if (args.Length == 1)
                Application.Run(new FormMain(args[0]));
            else
                throw new Exception("Unknown arguments");
#else
            try
            {
                if (args.Length == 0)
                    Application.Run(new FormMain());
                else if (args.Length == 1)
                    Application.Run(new FormMain(args[0]));
                else
                    throw new Exception("Unknown arguments");
            }
            catch (Exception ex)
            {
                string msg = ShellUtil.GetExceptionMsg(ex);
                MessageBox.Show(msg, "GEF", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
#endif
        }

        [STAThread]
        internal static void Main(string[] args)
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Run(args);
        }
    }
}
