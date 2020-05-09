using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using EcgApp.ViewModels;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.Mvc;
using System.Text.RegularExpressions;
using System.Collections;
using System.Security.Cryptography.Xml;

using System.Web;


namespace EcgApp.Controllers
{
    public class ChooseEcgRecordController : Controller
    {
        private readonly IWebHostEnvironment hostingEnvironment;

        public ChooseEcgRecordController(IWebHostEnvironment hostingEnvironment)
        {
            this.hostingEnvironment = hostingEnvironment;
        }

        // GET: /<controller>/
        public IActionResult ChooseEcgRecord()
        {
            return View();
        }

        [HttpPost]
        public IActionResult Ok(RecordViewModel model)
        {
            string uniqueFileName = null;
            string filePath = null;

            if (model.RecordFile != null)
            {
                string uploadsFolder = Path.Combine(hostingEnvironment.WebRootPath, "records");
                uniqueFileName = Guid.NewGuid().ToString() + "_" + model.RecordFile.FileName;
                filePath = Path.Combine(uploadsFolder, uniqueFileName);
                model.RecordFile.CopyTo(new FileStream(filePath, FileMode.Create));
            }

            //execute .py
            // create process info
            var psi = new ProcessStartInfo();
            psi.FileName = @"C:\Users\Professional\AppData\Local\Programs\Python\Python36\python.exe";

            //provide script and argument
            var script = @"C:\Users\Professional\Desktop\1studing\CourseWork3\course_work_app\Course_work\cnn_code_4_classes\predicting_4.py";
            var arg = model.RecordFile.FileName.Split(".")[0];

            psi.Arguments = $"{script} {arg}";

            //process configuration
            psi.UseShellExecute = false;
            psi.CreateNoWindow = true;
            psi.RedirectStandardOutput = true;
            psi.RedirectStandardError = true;

            //execute process and get output
            var errors = "";
            string results = "";

            using (var process = Process.Start(psi))
            {
                errors = process.StandardError.ReadToEnd();
                results = process.StandardOutput.ReadToEnd();
                //Console.WriteLine(results);
            }

            string data = null;
            using (var reader = new StreamReader(model.RecordFile.OpenReadStream()))
            {
                while (reader.Peek() >= 0)
                    data += reader.ReadLine();
                //data = reader.ReadToEnd();
            }

            List<int> listValues = new List<int>();

            string[] numbers = Regex.Split(data, @"\D+");
            foreach (string value in numbers)
            {
                if (!string.IsNullOrEmpty(value))
                {
                    int i = int.Parse(value);
                    listValues.Add(i);
                }
            }

            ViewBag.VALUES = listValues;
            ViewBag.RESULT = results;

            string[] allResults = results.Split(' ');
            string resN = allResults[2] + "%";
            string resRBBB = allResults[4] + "%";
            string resB = allResults[6] + "%";
            string resAFIB = allResults[8] + "%";

            ViewBag.N = resN;
            ViewBag.RBBB = resRBBB;
            ViewBag.B = resB;
            ViewBag.AFIB = resAFIB;


            string lineFromFile = "";
            FileStream fileStream = new FileStream(@"C:\Users\Professional\Desktop\1studing\CourseWork3\course_work_app\Course_work\cnn_code_4_classes\EcgApp\EcgApp\App_Data\sectorsFile.txt", FileMode.Open);
            using (StreamReader reader = new StreamReader(fileStream))
            {
                lineFromFile = reader.ReadLine();
            }

            string[] allSectors = lineFromFile.Split(';');
            List<string> listN = new List<string>();
            List<string> listRBBB = new List<string>();
            List<string> listB = new List<string>();
            List<string> listAFIB = new List<string>();
            for (int i = 0; i < allSectors.Length - 1; i++)
            {
                string[] val = allSectors[i].Split(':');
                if (val[2] == "0")
                    listN.Add(val[0] + ", " + val[1] + ";");
                else
                    if (val[2] == "1")
                    listRBBB.Add(val[0] + ", " + val[1] + ";");
                else
                    if (val[2] == "2")
                    listB.Add(val[0] + ", " + val[1] + ";");
                else
                    if (val[2] == "3")
                    listAFIB.Add(val[0] + ", " + val[1] + ";");

            }

            ViewBag.listN = listN;
            ViewBag.listRBBB = listRBBB;
            ViewBag.listB = listB;
            ViewBag.listAFIB = listAFIB;

            return View();

        }
    }
}
