package mapreduce.quickstart;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;

public class MaxTempWithCombiner {
    public static void main(String[] args) throws Exception {
        MRDriverFactory.init(args, true, MaxTempWithCombiner.class,
                MaxTempMapper.class, MaxTempReducer.class,
                Text.class, IntWritable.class,
                Text.class, IntWritable.class);
    }
}
