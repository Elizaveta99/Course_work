using Microsoft.AspNetCore.Http;
using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace EcgApp.ViewModels
{
    public class RecordViewModel
    {
        [Required]
        public string Name { get; set; }
        public IFormFile RecordFile { get; set; }

        [Required]
        public string TypeN { get; set; }
        [Required]
        public string TypeRBBB { get; set; }
        [Required]
        public string TypeAFIB { get; set; }
        [Required]
        public string TypeB { get; set; }
    }
}
