using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace EcgApp.Models
{
    public class Record
    {
        [Required]
        public string Name { get; set; }
        [Required]
        public string RecordPath { get; set; }
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
