package applications.avro;

import org.apache.avro.Schema;
import org.apache.avro.generic.GenericData;
import org.apache.avro.generic.GenericDatumReader;
import org.apache.avro.generic.GenericDatumWriter;
import org.apache.avro.generic.GenericRecord;
import org.apache.avro.io.*;
import org.apache.avro.specific.SpecificDatumReader;
import org.apache.avro.specific.SpecificDatumWriter;
import org.junit.Test;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.util.Arrays;

public class StringPairTest {
    @Test
    public void test1() throws IOException {
        // 解析模式
        Schema.Parser parser = new Schema.Parser();
        Schema schema = parser.parse(StringPairTest.class.getResourceAsStream("/avro/StringPair.avsc"));
        // 创建Avro Record的实例
        GenericRecord datum = new GenericData.Record(schema);
        datum.put("left", "L");
        datum.put("right", "R");
        // 将实例序列化到输出流（字节）中
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        DatumWriter<GenericRecord> writer = new GenericDatumWriter<>(schema);
        Encoder encoder = EncoderFactory.get().binaryEncoder(out, null);
        writer.write(datum, encoder);
        encoder.flush();
        out.close();
        // 展示序列化
        System.out.println(Arrays.toString(out.toByteArray()));
        // 反序列化：从字节缓冲区读回对象
        DatumReader<GenericRecord> reader = new GenericDatumReader<GenericRecord>(schema);
        Decoder decoder = DecoderFactory.get().binaryDecoder(out.toByteArray(), null);
        GenericRecord result = reader.read(null, decoder);
        System.out.println("left: " + result.get("left"));
        System.out.println("right: " + result.get("right"));
    }

    @Test
    public void test2() throws IOException {
        // 解析模式
        Schema.Parser parser = new Schema.Parser();
        Schema schema = parser.parse(StringPairTest.class.getResourceAsStream("/avro/StringPair.avsc"));
        Schema newSchema = parser.parse(StringPairTest.class.getResourceAsStream("/avro/NewStringPair.avsc"));
        // 创建Avro Record的实例
        GenericRecord datum = new GenericData.Record(schema);
        datum.put("left", "L");
        datum.put("right", "R");
        // 将实例序列化到输出流（字节）中
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        DatumWriter<GenericRecord> writer = new GenericDatumWriter<>(schema);
        Encoder encoder = EncoderFactory.get().binaryEncoder(out, null);
        writer.write(datum, encoder);
        encoder.flush();
        out.close();
        // 反序列化：使用新模式从字节缓冲区读回对象
        DatumReader<GenericRecord> reader = new GenericDatumReader<>(schema, newSchema);
        Decoder decoder = DecoderFactory.get().binaryDecoder(out.toByteArray(), null);
        GenericRecord result = reader.read(null, decoder);
        System.out.println("left: " + result.get("left"));
        System.out.println("right: " + result.get("right"));
        System.out.println("description: " + result.get("description"));
    }

    @Test
    public void test3() throws IOException {
        // 解析模式
        Schema.Parser parser = new Schema.Parser();
        Schema schema = parser.parse(StringPairTest.class.getResourceAsStream("/avro/StringPair.avsc"));
        Schema newSchema = parser.parse(StringPairTest.class.getResourceAsStream("/avro/DeleteStringPair.avsc"));
        // 创建Avro Record的实例
        GenericRecord datum = new GenericData.Record(schema);
        datum.put("left", "L");
        datum.put("right", "R");
        // 将实例序列化到输出流（字节）中
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        DatumWriter<GenericRecord> writer = new GenericDatumWriter<>(schema);
        Encoder encoder = EncoderFactory.get().binaryEncoder(out, null);
        writer.write(datum, encoder);
        encoder.flush();
        out.close();
        // 反序列化：使用新模式从字节缓冲区读回对象
        DatumReader<GenericRecord> reader = new GenericDatumReader<>(schema, newSchema);
        Decoder decoder = DecoderFactory.get().binaryDecoder(out.toByteArray(), null);
        GenericRecord result = reader.read(null, decoder);
        System.out.println("right: " + result.get("right"));
    }

    @Test
    public void test4() throws IOException {
        // 创建Avro对象实例
        StringPair datum = new StringPair();
        datum.setLeft("L");
        datum.setRight("R");
        // 将实例序列化到输出流（字节）中
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        DatumWriter<StringPair> writer = new SpecificDatumWriter<>(StringPair.class);
        Encoder encoder = EncoderFactory.get().binaryEncoder(out, null);
        writer.write(datum, encoder);
        encoder.flush();
        out.close();
        // 展示序列化
        System.out.println(Arrays.toString(out.toByteArray()));
        // 反序列化：从字节缓冲区读回对象
        DatumReader<StringPair> reader = new SpecificDatumReader<>(StringPair.class);
        Decoder decoder = DecoderFactory.get().binaryDecoder(out.toByteArray(), null);
        StringPair result = reader.read(null, decoder);
        System.out.println("left: " + result.get("left"));
        System.out.println("right: " + result.get("right"));
    }
}

