package hdfs.io;

import org.apache.hadoop.io.*;
import org.apache.hadoop.util.StringUtils;
import org.junit.Test;

import java.io.*;

public class Serialize {
    public byte[] serialize(Writable w) throws IOException {
        /*
            ByteArrayOutputStream是OutputStream（字节输出流）的子类。
            This class implements an output stream in which the data is written into a byte array.
            The buffer automatically grows as data is written to it.
            The data can be retrieved using toByteArray() and toString().
            Closing a ByteArrayOutputStream has no effect.
            The methods in this class can be called after the stream has been closed without generating an IOException.
         */
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        /*
            用于封装字节流，用于直接输出Java基本类型。
            A data output stream lets an application write primitive Java data types to an output stream in a portable way.
         */
        DataOutputStream dataOut = new DataOutputStream(out);
        w.write(dataOut);
        dataOut.close();
        return out.toByteArray();
    }

    public byte[] deserialize(Writable w, byte[] bytes) throws IOException {
        ByteArrayInputStream in = new ByteArrayInputStream(bytes);
        DataInputStream dataIn = new DataInputStream(in);
        w.readFields(dataIn);
        dataIn.close();
        return bytes;
    }

    @Test
    public void serializeInt() throws IOException {
        IntWritable writable = new IntWritable();
        writable.set(163); // 二进制表示为：1010 0011 十六进制表示为：000000a3
        byte[] bytes = serialize(writable);
        System.out.printf("Bytes length: %d\n", bytes.length);
        System.out.printf("Bytes hex: %s\n", StringUtils.byteToHexString(bytes));
    }

    @Test
    public void deserializeInt() throws IOException {
        IntWritable writable = new IntWritable();
        byte[] bytes = new byte[]{0, 0, 105, (byte) 231}; // 十六进制表示为：69e7 十进制表示为：27111
        deserialize(writable, bytes);
        System.out.printf("Writable value: %d\n", writable.get());
    }

    @Test
    public void compare() throws IOException {
        IntWritable iw1 = new IntWritable(163);
        IntWritable iw2 = new IntWritable(67);
        WritableComparator wc = WritableComparator.get(IntWritable.class);
        System.out.println("iw1 > iw2: " + (wc.compare(iw1, iw2) > 0 ? "true" : "false"));
        byte[] is1 = serialize(iw1);
        byte[] is2 = serialize(iw2);
        System.out.println("is1 > is2: " + (wc.compare(is1, 0, is1.length, is2, 0, is2.length) > 0 ? "true" : "false"));
    }

    @Test
    public void vInt() throws IOException {
        VIntWritable writable = new VIntWritable();
        writable.set(163); // 二进制表示为：1010 0011 十六进制表示为：000000a3 1000 1111
        byte[] bytes = serialize(writable);
        System.out.printf("Bytes length: %d\n", bytes.length);
        System.out.printf("Bytes hex: %s\n", StringUtils.byteToHexString(bytes));
        writable.set(27111); // 十六进制表示为：69e7
        bytes = serialize(writable);
        System.out.printf("Bytes length: %d\n", bytes.length);
        System.out.printf("Bytes hex: %s\n", StringUtils.byteToHexString(bytes));
    }

    @Test
    public void vIntLength() throws IOException {
        VIntWritable writable = new VIntWritable(163);
        byte[] bytes = serialize(writable);
        System.out.printf("Bytes length: %d\n", WritableUtils.decodeVIntSize(bytes[0]));
        writable.set(27111); // 十六进制表示为：69e7
        bytes = serialize(writable);
        System.out.printf("Bytes length: %d\n", WritableUtils.decodeVIntSize(bytes[0]));
    }

    @Test public void textIndex() {
        Text text = new Text("hadoop");
        System.out.println("string length: " + text.getLength());
        System.out.println("bytes length: " + text.getBytes().length);
        System.out.println("2th char (numeric): " + text.charAt(2));
        System.out.println("2th char (character): " + (char) text.charAt(2));
        System.out.println("d's index: " + text.find("d "));
    }
}
