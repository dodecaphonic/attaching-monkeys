using System;
using System.Runtime.InteropServices;

namespace AmazingPlugins
{
    public class DrawingBridge
    {
        private readonly IntPtr cside_;

        public DrawingBridge(IntPtr cside)
        {
            cside_ = cside;
        }

        [DllImport("__Internal", EntryPoint="draw_square")]
        static extern void DrawSquare(IntPtr p, double x, double y, int size);

        [DllImport("__Internal", EntryPoint="start_drawing_operation")]
        static extern void StartDrawingOperation(IntPtr p);

        [DllImport("__Internal", EntryPoint="end_drawing_operation")]
        static extern void EndDrawingOperation(IntPtr p);
        
        public void DrawSquare(double x, double y, int size)
        {
            DrawingBridge.DrawSquare(cside_, x, y, size);
        }

        public void StartDrawingOperation()
        {
            DrawingBridge.StartDrawingOperation(cside_);
        }

        public void EndDrawingOperation()
        {
            DrawingBridge.EndDrawingOperation(cside_);
        }
    }
}
