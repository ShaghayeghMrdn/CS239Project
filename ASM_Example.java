import java.io.FileInputStream;
import java.io.FileOutputStream;

import org.objectweb.asm.ClassReader;
import org.objectweb.asm.ClassWriter;

import org.objectweb.asm.ClassVisitor;
import org.objectweb.asm.MethodVisitor;
import org.objectweb.asm.Opcodes;

import java.io.IOException;

class ASM_Example {
    public static void main(String[] args) {
        try {
            FileInputStream input_bytecode = new FileInputStream(args[0]);
            FileOutputStream output_bytecode = new FileOutputStream(args[1]);

            ClassReader reader = new ClassReader(input_bytecode);
            ClassWriter writer = new ClassWriter(ClassWriter.COMPUTE_FRAMES);

            //********************************************************************************
            // wrap ClassWriter using MyClassVisitor
            // accept MyClassVisitor to visit ClassReader
            ClassVisitor visitor = new MyClassVisitor(writer);
            reader.accept(visitor, 0);
            //********************************************************************************

            output_bytecode.write(writer.toByteArray());
            input_bytecode.close();
            output_bytecode.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

//********************************************************************************
// MyClassVisitor: user-defined ClassVisitor
// MyClassVisitor overrides the visitMethod method to return user-defined MethodVisitor

class MyClassVisitor extends ClassVisitor implements Opcodes {
    public MyClassVisitor(final ClassVisitor cv) {
        super(ASM5, cv);
    }
    @Override
    public MethodVisitor visitMethod(int access, String name, String desc, String signature, String[] exceptions) {
        MethodVisitor mv = cv.visitMethod(access, name, desc, signature, exceptions);
        if (mv == null) return null;
        return new MyMethodVisitor(mv);
    }
}
//********************************************************************************

//********************************************************************************
// MyMethodVisitor: user-defined MethodVisitor
// MyMethodVisitor overrides the visitMethodInsn method to prepare for bytecode instruction instrumentation
class MyMethodVisitor extends MethodVisitor implements Opcodes {
    public MyMethodVisitor(MethodVisitor mv) {
        super(ASM5, mv);
    }

    @Override
    // Now we add instrumentation code for printing
    public void visitMethodInsn(int opcode, String owner, String name, String desc, boolean itf) {
        //********************************************************************************
        // print "[Call begin] ..."
        // use ASMifier to generate instrumentation code for your need
        mv.visitFieldInsn(GETSTATIC, "java/lang/System", "out", "Ljava/io/PrintStream;");
        mv.visitLdcInsn("[Call begin] " + owner + "::" + name);
        mv.visitMethodInsn(INVOKEVIRTUAL, "java/io/PrintStream", "println", "(Ljava/lang/String;)V", false);
        //********************************************************************************

        mv.visitMethodInsn(opcode, owner, name, desc, itf);

        //********************************************************************************
        // print "[Call end] ..."
        mv.visitFieldInsn(GETSTATIC, "java/lang/System", "out", "Ljava/io/PrintStream;");
        mv.visitLdcInsn("[Call end] " + owner + "::" + name);
        mv.visitMethodInsn(INVOKEVIRTUAL, "java/io/PrintStream", "println", "(Ljava/lang/String;)V", false);
        //********************************************************************************
    }
}
//********************************************************************************

