package hdfs.io;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IOUtils;
import org.apache.hadoop.io.SequenceFile;
import org.apache.hadoop.io.Writable;
import org.apache.hadoop.util.ReflectionUtils;

import java.io.IOException;

public class SequenceFileReadDemo {

    public static void main(String[] args) throws IOException {
        // Setup configurations
        Configuration conf = new Configuration();
        Path path = new Path(args[0]);

        // Transform to SequenceFile.Writer.Option
        SequenceFile.Reader.Option optPath = SequenceFile.Reader.file(path);

        // Read file
        SequenceFile.Reader reader = null;
        try {
            reader = new SequenceFile.Reader(conf, optPath);
            Writable key = (Writable) ReflectionUtils.newInstance(reader.getKeyClass(), conf);
            Writable value = (Writable) ReflectionUtils.newInstance(reader.getValueClass(), conf);
            long position = reader.getPosition(); // 每一对KV开始的位置，上一对KV结束的位置
            while (reader.next(key, value)) {
                /*
                    syncSeen(): Returns true iff the previous call to next passed a sync mark.
                */
                String syncSeen = reader.syncSeen() ? "*" : "";
                System.out.printf("[%s%s]\t%s\t%s\n", position, syncSeen, key, value);
                position = reader.getPosition(); // beginning of next record
            }
        } finally {
            IOUtils.closeStream(reader);
        }
    }
}