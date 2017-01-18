using System;
using System.Runtime.InteropServices;

namespace PhaseZero
{
    /// <summary>
    /// P/Invoke functions.
    /// </summary>
    internal static class PInvoke
    {
        /// <summary>
        /// https://msdn.microsoft.com/en-us/library/windows/desktop/ms632680(v=vs.85).aspx
        /// </summary>
        [DllImport("user32.dll", EntryPoint = "CreateWindowEx", CharSet = CharSet.Unicode)]
        internal static extern IntPtr CreateWindowEx(
            int dwExStyle,
            string lpszClassName,
            string lpszWindowName,
            int style,
            int x,
            int y,
            int width,
            int height,
            IntPtr hwndParent,
            IntPtr hMenu,
            IntPtr hInst,
            [MarshalAs(UnmanagedType.AsAny)] object pvParam);


        /// <summary>
        /// https://msdn.microsoft.com/en-us/library/windows/desktop/ms632682(v=vs.85).aspx
        /// </summary>
        [DllImport("user32.dll", EntryPoint = "DestroyWindow", CharSet = CharSet.Unicode)]
        internal static extern bool DestroyWindow(IntPtr hwnd);


        /// <summary>
        /// https://msdn.microsoft.com/en-us/library/windows/desktop/ms633576(v=vs.85).aspx
        /// </summary>
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        internal struct WNDCLASS
        {
            public uint style;
            public IntPtr lpfnWndProc;
            public int cbClsExtra;
            public int cbWndExtra;
            public IntPtr hInstance;
            public IntPtr hIcon;
            public IntPtr hCursor;
            public IntPtr hbrBackground;
            [MarshalAs(UnmanagedType.LPWStr)]
            public string lpszMenuName;
            [MarshalAs(UnmanagedType.LPWStr)]
            public string lpszClassName;
        }


        /// <summary>
        /// https://msdn.microsoft.com/en-us/library/windows/desktop/ms633586(v=vs.85).aspx
        /// </summary>
        [DllImport("user32.dll", SetLastError = true)]
        internal static extern ushort RegisterClassW([In] ref WNDCLASS lpWndClass);


        /// <summary>
        /// https://msdn.microsoft.com/en-us/library/windows/desktop/ms633572(v=vs.85).aspx
        /// </summary>
        [DllImport("user32.dll", SetLastError = true)]
        internal static extern IntPtr DefWindowProcW(IntPtr hWnd, uint msg, IntPtr wParam, IntPtr lParam);


        /// <summary>
        /// https://msdn.microsoft.com/it-it/library/windows/desktop/ms648391(v=vs.85).aspx
        /// </summary>
        [DllImport("user32.dll", SetLastError = true)]
        internal static extern IntPtr LoadCursor(IntPtr hInstance, int lpCursorName);
    }
}
