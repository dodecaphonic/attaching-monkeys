using Tao.OpenGl;
using System;

namespace AmazingPlugins
{
    class AmazingGLPlugin : IMouseMovePlugin, IGLPlugin
    {
        readonly DrawingBridge bridge_;
        private const int FACES = 6;

        private static bool polySmooth = true;

        private static float[/*8*/, /*3*/] v = new float [8, 3];
        private static float[/*8*/, /*4*/] c = {
            {0.0f, 0.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 0.0f, 1.0f},
            {0.0f, 1.0f, 0.0f, 1.0f},
            {1.0f, 1.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f, 1.0f},
            {1.0f, 0.0f, 1.0f, 1.0f},
            {0.0f, 1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f, 1.0f}
        };

        // indices of front, top, left, bottom, right, back faces
        private static byte[/*6*/, /*4*/] indices = {
            {4, 5, 6, 7},
            {2, 3, 7, 6},
            {0, 4, 7, 3},
            {0, 1, 5, 4},
            {1, 5, 6, 2},
            {0, 3, 2, 1}
        };
        
        public AmazingGLPlugin(DrawingBridge bridge)
        {
            bridge_ = bridge;
            Configure();
        }

        public string Name {
            get { return "Amazing GL Plugin"; }
        }

        public void Configure()
        {
            Gl.glCullFace(Gl.GL_BACK);
            Gl.glEnable(Gl.GL_CULL_FACE);
            Gl.glBlendFunc(Gl.GL_SRC_ALPHA_SATURATE, Gl.GL_ONE);
            Gl.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        }
        
        private void DrawCube(float x0, float x1, float y0, float y1, float z0, float z1) {
            v[0, 0] = v[3, 0] = v[4, 0] = v[7, 0] = x0;
            v[1, 0] = v[2, 0] = v[5, 0] = v[6, 0] = x1;
            v[0, 1] = v[1, 1] = v[4, 1] = v[5, 1] = y0;
            v[2, 1] = v[3, 1] = v[6, 1] = v[7, 1] = y1;
            v[0, 2] = v[1, 2] = v[2, 2] = v[3, 2] = z0;
            v[4, 2] = v[5, 2] = v[6, 2] = v[7, 2] = z1;

            Gl.glEnable(Gl.GL_POINT_SMOOTH);
            Gl.glEnableClientState(Gl.GL_VERTEX_ARRAY);
            Gl.glEnableClientState(Gl.GL_COLOR_ARRAY);
            Gl.glVertexPointer(3, Gl.GL_FLOAT, 0, v);
            Gl.glColorPointer(4, Gl.GL_FLOAT, 0, c);
            Gl.glDrawElements(Gl.GL_QUADS, FACES * 4, Gl.GL_UNSIGNED_BYTE, indices);
            Gl.glDisableClientState(Gl.GL_VERTEX_ARRAY);
            Gl.glDisableClientState(Gl.GL_COLOR_ARRAY);
        }

        private void Display() {
            bridge_.StartDrawingOperation();
            // if(polySmooth) {
            //     Gl.glClear(Gl.GL_COLOR_BUFFER_BIT);
            //     Gl.glEnable(Gl.GL_BLEND);
            //     Gl.glEnable(Gl.GL_POLYGON_SMOOTH);
            //     Gl.glDisable(Gl.GL_DEPTH_TEST);
            // }
            // else {
            //     Gl.glClear(Gl.GL_COLOR_BUFFER_BIT | Gl.GL_DEPTH_BUFFER_BIT);
            //     Gl.glDisable(Gl.GL_BLEND);
            //     Gl.glDisable(Gl.GL_POLYGON_SMOOTH);
            //     Gl.glEnable(Gl.GL_DEPTH_TEST);
            // }

            // Gl.glPushMatrix();
            // Gl.glTranslatef(0.0f, 0.0f, -8.0f);    
            // Gl.glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
            // Gl.glRotatef(60.0f, 0.0f, 1.0f, 0.0f); 
            // DrawCube(-0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f);
            // Gl.glPopMatrix ();

            // Gl.glFlush ();
            Random rand = new Random();
            float r, g, b;
            r = (float)(rand.Next(0, 255) / 255.0);
            g = (float)(rand.Next(0, 255) / 255.0);
            b = (float)(rand.Next(0, 255) / 255.0);
            Gl.glClear(Gl.GL_COLOR_BUFFER_BIT | Gl.GL_DEPTH_BUFFER_BIT);
            Gl.glColor3f(r, g, b);
            Gl.glRectf(-0.75f,0.75f, 0.75f, -0.75f);
            
            bridge_.EndDrawingOperation();
        }

        private void Reshape(int w, int h) {
            bridge_.StartDrawingOperation();
            Gl.glViewport(0, 0, w, h);
            Gl.glMatrixMode(Gl.GL_PROJECTION);
            Gl.glLoadIdentity();
            Glu.gluPerspective(30.0, (float) w / (float) h, 1.0, 20.0);
            Gl.glMatrixMode(Gl.GL_MODELVIEW);
            Gl.glLoadIdentity();
            bridge_.EndDrawingOperation();
        }        

        public void ReceiveEvent(IDomain domain, object[] data)
        {
            Display();
        }
    }
}

