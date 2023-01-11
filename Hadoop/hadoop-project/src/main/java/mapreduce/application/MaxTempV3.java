package mapreduce.application;

import mapreduce.quickstart.MRDriverFactory;
import mapreduce.quickstart.MaxTempMapper;
import mapreduce.quickstart.MaxTempReducer;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;

public class MaxTempV3 {

    public static void main(String[] args) throws Exception {
        MRDriverFactory.init(args, false, MaxTempV3.class,
                MaxTemperatureMapperV3.class, MaxTempReducer.class,
                Text.class, IntWritable.class,
                Text.class, IntWritable.class);
    }
}