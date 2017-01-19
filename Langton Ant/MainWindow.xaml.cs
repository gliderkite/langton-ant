using Direct2DInterop;
using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Threading;

namespace lant
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        /// <summary>
        /// Langton's ant engine.
        /// </summary>
        private LAntEngine engine;

        /// <summary>
        /// Most recent mouse position after mouse down event.
        /// </summary>
        private Point mousedown;

        /// <summary>
        /// Most recent mouse position after mouse up event.
        /// </summary>
        private Point mouseup;

        /// <summary>
        /// Timer used to go to the next generation step.
        /// </summary>
        private readonly DispatcherTimer timer = new DispatcherTimer();



        /// <summary>
        /// Constructor.
        /// </summary>
        public MainWindow()
        {
            InitializeComponent();

            // initialize the timer
            timer.Tick += DispatcherTimer_Tick;
            timer.Interval = TimeSpan.FromMilliseconds(100);            
        }



        #region Events


        /// <summary>
        /// Dispatcher timer tick event.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void DispatcherTimer_Tick(object sender, EventArgs e)
        {
            try
            {
                if (engine != null)
                {
                    engine.Step();
                    engine.Refresh();
                }
            }
            catch
            { }
        }


        /// <summary>
        /// Thie event signals the viewport host window has been properly created.
        /// </summary>
        /// <param name="hwndHost">Pointer to the native HWND host window.</param>
        private void Viewport_BuildWindowCoreCompleted(IntPtr hwndHost)
        {
            try
            {
                Debug.Assert(hwndHost != IntPtr.Zero);
                Debug.Assert(engine == null);
                engine = new LAntEngine(hwndHost);
                engine.SetOffset((float)Width / 2, (float)Height / 2);

                // subscribe to the WndProc
                Viewport.ProcessMessage += Viewport_ProcessMessage;

                Debug.Assert(timer != null);
                timer.Start();
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message, "Unhandled exception!", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
        

        /// <summary>
        /// Custom window procedure event handler.
        /// </summary>
        /// <param name="hWnd">A handle to the window.</param>
        /// <param name="msg">The message.</param>
        /// <param name="wParam">Additional message information.</param>
        /// <param name="lParam">Additional message information.</param>
        /// <returns></returns>
        private IntPtr Viewport_ProcessMessage(IntPtr hWnd, uint msg, IntPtr wParam, IntPtr lParam)
        {
            if (engine != null)
            {
                int lowWord = lParam.ToInt32() & 0xffff;
                int highWord = (lParam.ToInt32() >> 16) & 0xffff;

                // WM_MOUSEMOVE
                if (msg == 0x0200)
                {
                    // MK_LBUTTON
                    if (wParam.ToInt32() == 0x0001)
                    {
                        // get mouse position within the client rect
                        mouseup.X = lowWord;
                        mouseup.Y = highWord;

                        // compute delta
                        float offx, offy;
                        engine.GetOffset(out offx, out offy);
                        float x = (float)(mouseup.X - mousedown.X);
                        float y = (float)(mouseup.Y - mousedown.Y);
                        float zoom = engine.GetZoom();
                        Debug.Assert(zoom > 0);

                        // set the offset and refresh the viewport
                        engine.SetOffset(offx + x / zoom, offy + y / zoom);
                        engine.Refresh();

                        // reset mouse delta
                        mousedown = mouseup;
                    }
                }
                // WM_LBUTTONDOWN
                else if (msg == 0x0201)
                {
                    // get mouse position within the client rect
                    mousedown.X = lowWord;
                    mousedown.Y = highWord;
                    mouseup = mousedown;
                }
                // WM_MOUSEWHEEL
                else if (msg == 0x020A)
                {
                    // compute the new zoom 
                    float zDelta = (short)((ushort)((((ulong)(wParam.ToInt64())) >> 16) & 0xffff));
                    float z = engine.GetZoom() * (zDelta > 0 ? 1.75f : (1 / 1.75f));
                    var pt = Viewport.PointFromScreen(new Point(lowWord, highWord));

                    // set the zoom and refresh the viewport
                    engine.SetZoom(z, (float)pt.X, (float)pt.Y);
                    engine.Refresh();
                }
                // WM_SIZE
                else if (msg == 0x0005)
                {
                    engine.Resize();                    
                    engine.Refresh();
                }
            }

            return PInvoke.DefWindowProcW(hWnd, msg, wParam, lParam);
        }


        #endregion


    }
}
