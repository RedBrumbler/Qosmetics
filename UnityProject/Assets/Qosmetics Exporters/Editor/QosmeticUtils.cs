using UnityEngine;
using UnityEditor;
using System.IO;
using System.IO.Compression;
using System;
using Boo.Lang;

namespace Qosmetics
{
    public class QosmeticUtils
    {
        public static void adb(string command)
        {
            Debug.Log("Executing command: " + command);
            System.Diagnostics.Process.Start("adb.exe", command);
        }

        // credit for SetLayerRecursively function goes to @Bobbie#0001
        public static void SetLayerRecursively(GameObject obj, int newLayer)
        {
            if (obj == null)
            {
                return;
            }
            obj.layer = newLayer;
            foreach (Transform child in obj.transform)
            {
                if (child == null)
                {
                    continue;
                }
                SetLayerRecursively(child.gameObject, newLayer);
            }
        }
        /// <summary>
        /// Exports SaberConfig config as path
        /// </summary>
        /// <param name="config"></param>
        /// <param name="path"></param>
        public static void exportSaberConfig(SaberConfig config, string path)
        {
            TextAsset saberjson = new TextAsset(JsonUtility.ToJson(config, true));
            AssetDatabase.CreateAsset(saberjson, path);
        }

        /// <summary>
        /// Exports WallConfig config as path
        /// </summary>
        /// <param name="config"></param>
        /// <param name="path"></param>
        public static void exportWallConfig(WallConfig config, string path)
        {
            TextAsset walljson = new TextAsset(JsonUtility.ToJson(config, true));
            AssetDatabase.CreateAsset(walljson, path);
        }
        /// <summary>
        /// Exports BloqConfig config as path
        /// </summary>
        /// <param name="config"></param>
        /// <param name="path"></param>
        public static void exportBloqConfig(BloqConfig config, string path)
        {
            TextAsset walljson = new TextAsset(JsonUtility.ToJson(config, true));
            AssetDatabase.CreateAsset(walljson, path);
        }

        /// <summary>
        /// Exports mod bmbfmod as path
        /// </summary>
        /// <param name="bmbfmod"></param>
        /// <param name="path"></param>
        public static void exportBMBFasset(BMBFmod.mod bmbfmod, string path)
        {
            TextAsset bmbfjson = new TextAsset(JsonUtility.ToJson(bmbfmod, true));
            AssetDatabase.CreateAsset(bmbfjson, path);
        }

        /// <summary>
        /// Creates a thumbnail from the given camera
        /// </summary>
        /// <param name="cam"></param>
        /// <param name="path"></param>
        public static void exportImage(Texture2D image, string path)
        {
            byte[] bytes;

            bytes = image.EncodeToPNG();

            File.WriteAllBytes(path, bytes);
        }

        /// <summary>
        /// Returns a Texture2D of what the given camera sees
        /// </summary>
        /// <param name="cam"></param>
        public static Texture2D getTexture2D(Camera cam, int width = 1024, int height = 512)
        {
            RenderTexture tempRT = new RenderTexture(width, height, 24);

            tempRT.antiAliasing = 8;

            cam.targetTexture = tempRT;
            cam.Render();

            RenderTexture.active = tempRT;

            Texture2D image = new Texture2D(width, height, TextureFormat.RGB24, false);

            image.ReadPixels(new Rect(0, 0, width, height), 0, 0);

            RenderTexture.active = null;

            cam.targetTexture = null;
            UnityEngine.Object.DestroyImmediate(tempRT);

            return image;
        }

        /// <summary>
        /// zips src folder and output gets saved as dst, then deletes all files in src and below, so make sure it's not an important directory!
        /// </summary>
        /// <param name="src"></param>
        /// <param name="dst"></param>
        public static void createZipFromFolder(string src, string dst)
        {
            ZipFile.CreateFromDirectory(src, dst);

            DirectoryInfo di = new DirectoryInfo(src);

            foreach (FileInfo file in di.GetFiles())
            {
                file.Delete();
            }

            foreach (DirectoryInfo dir in di.GetDirectories())
            {
                dir.Delete();
            }

            Directory.Delete(src);
        }
        /// <summary>
        /// Replaces certain characters with empty strings to make filenames neater
        /// </summary>
        /// <param name="input"></param>
        /// <returns></returns>
        public static string sanitizeString(string input)
        {
            string output = input.Replace(" ", String.Empty);
            output = output.Replace("'", String.Empty);
            return output;
        }

        /// <summary>
        /// checks to see if the description or component is null and if so adds 1 to their length
        /// </summary>
        /// <param name="mod"></param>
        public static void addIfNull(BMBFmod.mod mod)
        {
            try
            {
                string temp = mod.components[0].type;
            }
            catch (NullReferenceException)
            {
                mod.components.Add(new BMBFmod.Component());
            }
            catch (ArgumentOutOfRangeException)
            {
                mod.components.Add(new BMBFmod.Component());
            }

            try
            {
                string temp = mod.description[0];
            }
            catch (NullReferenceException)
            {
                mod.description.Add("");
            }
            catch (ArgumentOutOfRangeException)
   
            {
                mod.description.Add("");
            }
        }

        /// <summary>
        /// Purges all objects of type Light in toBeCleansed object
        /// </summary>
        /// <param name="toBeCleansed"></param>
        public static void PurgeLights(GameObject toBeCleansed)
        {
            Component[] lights = toBeCleansed.GetComponentsInChildren<Light>();

            foreach (var light in lights)
            {
                UnityEngine.Object.DestroyImmediate(light);
            }
        }

        /// <summary>
        /// Purges all objects of type camera in toBeCleansed object
        /// </summary>
        /// <param name="toBeCleansed"></param>
        public static void PurgeCameras(GameObject toBeCleansed)
        {
            Component[] cameras = toBeCleansed.GetComponentsInChildren<Camera>();

            foreach (var camera in cameras)
            {
                UnityEngine.Object.DestroyImmediate(camera);
            }
        }

        /// <summary>
        /// Gets object bounds
        /// </summary>
        /// <param name="g"></param>
        /// <returns></returns>
        public static Bounds GetObjectBounds(GameObject g)
        {
            var b = new Bounds(g.transform.position, Vector3.zero);
            foreach (Renderer r in g.GetComponentsInChildren<Renderer>())
            {
                b.Encapsulate(r.bounds);
            }
            return b;
        }
    }
}

