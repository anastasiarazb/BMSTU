package ru.bmstu.nastasia;

import org.yeastrc.fasta.FASTAHeader;

import java.util.ArrayList;

public class Line {

    private String name;
    private String line;

    public Line(FASTAHeader header) {
        name = header.getName();
        line = header.getLine();
    }

    public Line(String name, String line) {
        this.line = line;
        this.name = name;
    }

    public static void prettyPrint(ArrayList<Line> lines, int outputWidth) {
        String[] names = new String[lines.size()];
        int maxLenName = 0;
        int maxLenLine   = 0;
        for (int i = 0; i < lines.size(); ++i) {
            maxLenName = Integer.max(maxLenName, lines.get(i).name.length());
            maxLenLine = Integer.max(maxLenLine, lines.get(i).line.length());

        }
        for (int i = 0; i < lines.size(); ++i) {
            names[i] = String.format("%-"+ maxLenName + "s",lines.get(i).name);
//            if (lines[i].name.length() < max_len) {
//                int delta = max_len - lines[i].name.length();
//                lines[i].name = lines[i].name + String.format("")
//            }
        }
        int start_pos;
        for (start_pos = 0; start_pos + outputWidth < maxLenLine; start_pos += outputWidth) {
            for (int i = 0; i < lines.size(); ++i) {
                String substr = "";
                try {
                    substr = lines.get(i).line.substring(start_pos, start_pos + outputWidth);
                } catch (StringIndexOutOfBoundsException ex) {
                    substr = "";
                }
                System.out.println(lines.get(i).name + ": " + substr);
            }
            System.out.println();

        }

        for (int i = 0; i < lines.size(); ++i) {
            String substr = "";
            try {
                substr = lines.get(i).line.substring(start_pos);
            } catch (StringIndexOutOfBoundsException ex) {
                substr = "";
            }
            System.out.println(lines.get(i).name + ": " + substr);
        }
    }

    // ---------------------- Getters and setters -------------------------

    public String getName() {
        return name;
    }

    public void setLine(String line) {
        this.line = line;
    }

    public String getLine() {
        return line;
    }
}
