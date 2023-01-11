package hdfs.io;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IOUtils;
import org.apache.hadoop.io.compress.*;
import org.apache.hadoop.util.ReflectionUtils;

import java.io.InputStream;
import java.io.OutputStream;
import java.net.URI;

public class PooledStreamCompressor {

    public static void main(String[] args) throws Exception {
        // 获取文件路径与FileSystem对象
        String uri = args[0];
        Configuration conf = new Configuration();
        FileSystem fs = FileSystem.get(URI.create(uri), conf);

        // 通过文件名获得Codec对象
        Path inputPath = new Path(uri);
        CompressionCodecFactory factory = new CompressionCodecFactory(conf);
        CompressionCodec codec = factory.getCodec(inputPath);
        if (codec == null) {
          System.err.println("No codec found for " + uri);
          System.exit(1);
        }

        // 解压文件并输出到控制台
        InputStream in = null;
        Decompressor decompressor = null;
        try {
          decompressor = CodecPool.getDecompressor(codec);
          in = codec.createInputStream(fs.open(inputPath), decompressor);
          IOUtils.copyBytes(in, System.out, conf);
        } finally {
          IOUtils.closeStream(in);
          CodecPool.returnDecompressor(decompressor);
        }
    }
}
