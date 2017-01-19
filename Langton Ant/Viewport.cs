using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Windows.Interop;

namespace Direct2DInterop
{
    /// <summary>
    /// Build Window Core delegate,
    /// </summary>
    /// <param name="hwndHost">Viewport host window.</param>
    public delegate void BuildWindowCoreDelegate(IntPtr hwndHost);

    /// <summary>
    /// Window main procedure delegate.
    /// </summary>
    /// <param name="hWnd">A handle to the window.</param>
    /// <param name="msg">The message.</param>
    /// <param name="wParam">Additional message information.</param>
    /// <param name="lParam">Additional message information.</param>
    /// <returns></returns>
    internal delegate IntPtr WndProcDelegate(IntPtr hWnd, uint msg, IntPtr wParam, IntPtr lParam);


    /// <summary>
    /// Represents the viewport.
    /// A custom control hosted by a WPF application which rendering can be
    /// redirect to a native DirectX application.
    /// </summary>
    sealed class Viewport : HwndHost
    {


        /// <summary>
        /// Raised only when the hosted window has been created properly.
        /// </summary>
        public event BuildWindowCoreDelegate BuildWindowCoreCompleted;

        /// <summary>
        /// Processes Windows messages.
        /// </summary>
        public static event WndProcDelegate ProcessMessage;

        /// <summary>
        /// Window procedure delegate.
        /// </summary>
        private WndProcDelegate wproc;

        /// <summary>
        /// Hosted window.
        /// </summary>
        private IntPtr hwndHost;
        


        /// <summary>
        /// Create the window to host.
        /// </summary>
        /// <param name="hwndParent">Parent window handle.</param>
        /// <returns></returns>
        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            // Register the window class.
            var wcex = new PInvoke.WNDCLASS();

            const uint CS_HREDRAW = 0x0002;
            const uint CS_VREDRAW = 0x0001;
            const int WS_CHILD = 0x40000000;
            const int WS_VISIBLE = 0x10000000;
            const int IDC_ARROW = 32512;
            const string className = "PhaseZeroViewportClass";

            wcex.style = CS_HREDRAW | CS_VREDRAW;
            wcex.lpszClassName = className;
            wcex.hCursor = PInvoke.LoadCursor(IntPtr.Zero, IDC_ARROW);
            wcex.lpfnWndProc = Marshal.GetFunctionPointerForDelegate(wproc = CustomWndProc);

            // register class and check failure
            if (PInvoke.RegisterClassW(ref wcex) == 0)
                return new HandleRef(this, IntPtr.Zero);
            
            // create the viewport host window
            hwndHost = PInvoke.CreateWindowEx(
                0,                      // extended style
                className,              // class name
                String.Empty,           // window name
                WS_CHILD | WS_VISIBLE,  // style
                0,                      // x
                0,                      // y
                0,                      // width
                0,                      // height
                hwndParent.Handle,      // parent window handle
                IntPtr.Zero,            // menu
                IntPtr.Zero,            // instance
                0);                     // params

            // invoke the event to signal the host window has been properly created
            if (hwndHost != IntPtr.Zero)
                BuildWindowCoreCompleted?.Invoke(hwndHost);

            return new HandleRef(this, hwndHost);
        }


        /// <summary>
        /// Destroy the hosted window.
        /// </summary>
        /// <param name="hwnd">Hosted window.</param>
        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            PInvoke.DestroyWindow(hwnd.Handle);
        }


        /// <summary>
        /// Custom window procedure.
        /// </summary>
        /// <param name="hWnd">A handle to the window.</param>
        /// <param name="msg">The message.</param>
        /// <param name="wParam">Additional message information.</param>
        /// <param name="lParam">Additional message information.</param>
        /// <returns></returns>
        private static IntPtr CustomWndProc(IntPtr hWnd, uint msg, IntPtr wParam, IntPtr lParam)
        {
            var ptr = ProcessMessage?.Invoke(hWnd, msg, wParam, lParam);

            if (!ptr.HasValue)
                ptr = PInvoke.DefWindowProcW(hWnd, msg, wParam, lParam);

            Debug.Assert(ptr.HasValue);
            return ptr.Value;
        }
        
        
    }
}
