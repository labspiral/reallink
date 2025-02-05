using System;
using System.Text.Json.Serialization;

namespace SharedModels
{
    public class Unit 
    {
        [JsonPropertyName("col")]
        public int Col { get; set; }
       
        [JsonPropertyName("row")]
        public int Row { get; set; }
       
        [JsonPropertyName("bin")]
        public int Bin
        {
            get { return _bin; }
            set
            {
                if (_bin != value)
                    _bin = value;
            }
        }
        private int _bin;

      
        public Unit(int col, int row, int bin)
        {
            Col = col;
            Row = row;
            Bin = bin;
        }

        public override string ToString()
        {
            return $"{Col},{Row}): [{Bin}]";
        }
    }
}
