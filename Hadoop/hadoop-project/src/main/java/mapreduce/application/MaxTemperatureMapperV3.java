package mapreduce.application;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

import java.io.IOException;

public class MaxTemperatureMapperV3 extends Mapper<LongWritable, Text, Text, IntWritable> {

    enum Temperature {
        OVER_100
    }

    private NcdcRecordParser parser = new NcdcRecordParser();

    @Override
    public void map(LongWritable key, Text value, Context context)
            throws IOException, InterruptedException {
        parser.parse(value);
        System.err.println("Temperature caught over degrees for input: " + parser.getAirTemperature());
        if (parser.isValidTemperature()) {
            int airTemperature = parser.getAirTemperature();
            if (airTemperature > 10) {
                System.err.println("Temperature over 1 degrees for input: " + value);
                context.setStatus("Detected possibly corrupt record: see logs.");
                context.getCounter(Temperature.OVER_100).increment(5);
            }
            context.write(new Text(parser.getYear()), new IntWritable(airTemperature));
        }
    }
}

