package mapreduce.application;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Counter;
import org.apache.hadoop.mapreduce.Counters;
import org.apache.hadoop.mrunit.mapreduce.MapDriver;
import org.junit.Test;

import java.io.IOException;

public class PrimeMapperTest {

    @Test
    public void processesValidRecord() throws IOException {
        Text value = new Text("101");
        Counters counters = new Counters();
        new MapDriver<LongWritable, Text, IntWritable, NullWritable>()
                .withMapper(new PrimeDebug())
                .withInput(new LongWritable(0), value)
                .withOutput(new IntWritable(101), NullWritable.get())
                .withCounters(counters)
                .runTest();
        Counter counter = counters.findCounter("MyCounter", "PRIME");
        System.out.println(counter.getValue());
    }
}
