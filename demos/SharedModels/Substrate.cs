using System;
using System.Collections.ObjectModel;
using System.Text.Json.Serialization;

namespace SharedModels
{
    public class Substrate 
    {
        [JsonPropertyName("name")]
        public string Name { get; set; }

        [JsonPropertyName("rows")]
        public int Rows { get; set; }

        [JsonPropertyName("cols")]
        public int Cols { get; set; }

        [JsonPropertyName("units")]
        public ObservableCollection<Unit> Units
        {
            get { return _units; }
            set
            {
                if (_units != value)
                    _units = value;
            }
        }
        ObservableCollection<Unit> _units;


        public Substrate()
        {
            _units = new ObservableCollection<Unit>();
        }
        public override string ToString()
        {
            return $"{Name}, rows: {Rows}, cols: {Cols}, units: {Units.Count}";
        }
    }
}
