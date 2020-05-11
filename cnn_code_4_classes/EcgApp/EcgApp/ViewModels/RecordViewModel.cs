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
        public List<string> TypeN { get; set; }
        [Required]
        public List<string> TypeRBBB { get; set; }
        [Required]
        public List<string> TypeAFIB { get; set; }
        [Required]
        public List<string> TypeB { get; set; }
    }
}
