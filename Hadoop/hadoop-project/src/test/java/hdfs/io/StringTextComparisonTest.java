package hdfs.io;

import org.apache.hadoop.io.Text;
import org.junit.Test;

import java.io.UnsupportedEncodingException;
import java.nio.charset.StandardCharsets;

public class StringTextComparisonTest {
    @Test
    public void string() {
        String s = "\u0041\u00DF\u6771\uD801\uDC00";
        System.out.println("Length: " + s.length());
        System.out.println("Byte length: " + s.getBytes(StandardCharsets.UTF_8).length);
        System.out.println("Index of \\u0041: " + s.indexOf("\u0041"));
        System.out.println("Index of \\u00DF: " + s.indexOf("\u00DF"));
        System.out.println("Index of \\u6771: " + s.indexOf("\u6771"));
        System.out.println("Index of \\uD801\\uDC00: " + s.indexOf("\uD801\uDC00"));
        System.out.println("Char at 0: " + s.charAt(0));
        System.out.println("Char at 1: " + s.charAt(1));
        System.out.println("Char at 2: " + s.charAt(2));
        System.out.println("Char at 3: " + s.charAt(3));
        System.out.println("Char at 4: " + s.charAt(4));
    /*
      Returns the character (Unicode code point) at the specified index.
      The index refers to char values (Unicode code units) and ranges from 0 to length()- 1.

      If the char value specified at the given index is in the high-surrogate range,
      the following index is less than the length of this String,
      and the char value at the following index is in the low-surrogate range,
      then the supplementary code point corresponding to this surrogate pair is returned.
      Otherwise, the char value at the given index is returned.
     */
        System.out.println("Code point at 0: " + Integer.toHexString(s.codePointAt(0)));
        System.out.println("Code point at 1: " + Integer.toHexString(s.codePointAt(1)));
        System.out.println("Code point at 2: " + Integer.toHexString(s.codePointAt(2)));
        System.out.println("Code point at 3: " + Integer.toHexString(s.codePointAt(3)));
    }

    @Test
    public void text() {
        Text t = new Text("\u0041\u00DF\u6771\uD801\uDC00");
        System.out.println("Length: " + t.getLength());
        System.out.println("Index of \\u0041: " + t.find("\u0041"));
        System.out.println("Index of \\u00DF: " + t.find("\u00DF"));
        System.out.println("Index of \\u6771: " + t.find("\u6771"));
        System.out.println("Index of \\uD801\\uDC00: " + t.find("\uD801\uDC00"));
        System.out.println("Code point at 0: " + Integer.toHexString(t.charAt(0)));
        System.out.println("Code point at 1: " + Integer.toHexString(t.charAt(1)));
        System.out.println("Code point at 3: " + Integer.toHexString(t.charAt(3)));
        System.out.println("Code point at 6: " + Integer.toHexString(t.charAt(6)));
    }
}
