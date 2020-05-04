using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using EcgApp.ViewModels;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.Mvc;

// For more information on enabling MVC for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace EcgApp.Controllers
{
    public class ChooseEcgRecordController : Controller
    {
        private readonly IHostingEnvironment hostingEnvironment;

        public ChooseEcgRecordController(IHostingEnvironment hostingEnvironment)
        {
            this.hostingEnvironment = hostingEnvironment;
        }

        // GET: /<controller>/
        public IActionResult ChooseEcgRecord()
        {
            return View();
        }

        [HttpPost]
        public string Ok(RecordViewModel model)
        {
            string uniqueFileName = null;

            if (model.RecordFile != null)
            {
                string uploadsFolder = Path.Combine(hostingEnvironment.WebRootPath, "records");
                uniqueFileName = Guid.NewGuid().ToString() + "_" + model.RecordFile.FileName;
                string filePath = Path.Combine(uploadsFolder, uniqueFileName);
                model.RecordFile.CopyTo(new FileStream(filePath, FileMode.Create));
            }

            //execute .py
            // create process info
            var psi = new ProcessStartInfo();
            //?? такой ли путь
            //psi.FileName = @"C:\Users\User\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Python 3.7";
            psi.FileName = @"C:\Users\Professional\AppData\Local\Programs\Python\Python36\python.exe";
            //psi.FileName = @"C: \Users\User\AppData\Local\Programs\Python\Python37 - 32\python.exe";
            //psi.FileName = @"C: \Users\User\AppData\Local\Programs\Python\Python37 - 32\python.exe";
        //provide script and argument
            var script = @"C:\Users\Professional\Desktop\1studing\CourseWork3\course_work_app\cnn_code_4_classes\predicting_4.py";
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

            using(var process = Process.Start(psi))
            {
                errors = process.StandardError.ReadToEnd();
                results = process.StandardOutput.ReadToEnd();
                Console.WriteLine(results);
            }

            //display output
            //return "ok";
            return arg + " " + results;

            //return View();

        }
    }
}
