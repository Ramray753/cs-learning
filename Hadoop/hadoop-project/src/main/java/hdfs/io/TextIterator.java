package hdfs.io;

import org.apache.hadoop.io.MapWritable;
import org.apache.hadoop.io.ObjectWritable;
import org.apache.hadoop.io.Text;

/*
    java.nio.Buffer继承自java.nio.Buffer：A container for data of a specific primitive type.
    A buffer is a linear, finite sequence of elements of a specific primitive type.
    Aside from its content, the essential properties of a buffer are its capacity, limit, and position:

      1. A buffer's capacity is the number of elements it contains.
      The capacity of a buffer is never negative and never changes.

      2. A buffer's limit is the index of the first element that should not be read or written.
      A buffer's limit is never negative and is never greater than its capacity.

      3, A buffer's position is the index of the next element to be read or written.
      A buffer's position is never negative and is never greater than its limit.

    There is one subclass of this class for each non-boolean primitive type.
*/
import java.nio.ByteBuffer;

public class TextIterator {

    public static void main(String[] args) {
        Text t = new Text("\u0041\u00DF\u6771\uD801\uDC00");
        /*
            ByteBuffer.wrap(): Wraps a byte array into a buffer.
            The new buffer will be backed by the given byte array;
            that is, modifications to the buffer will cause the array to be modified and vice versa.
            The new buffer's capacity will be array.length, its position will be offset,
            its limit will be offset + length, and its mark will be undefined.
            Its backing array will be the given array, and its array offset will be zero.
         */
        ByteBuffer buf = ByteBuffer.wrap(t.getBytes(), 0, t.getLength());
        int cp;
        /*
            hasRemaining(): Tells whether there are any elements between the current position and the limit.

            Text.bytesToCodePoint(): Returns the next code point at the current position in the buffer.
            The buffer's position will be incremented. Any mark set on this buffer will be changed by this method!
         */
        while (buf.hasRemaining() && (cp = Text.bytesToCodePoint(buf)) != -1) {
            System.out.println(Integer.toHexString(cp));
        }
    }
}
