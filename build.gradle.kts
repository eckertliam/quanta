plugins {
    id("java")
    id("antlr")
}

group = "org.quanta"
version = "1.0-SNAPSHOT"

repositories {
    mavenCentral()
}

dependencies {
    testImplementation(platform("org.junit:junit-bom:5.9.1"))
    testImplementation("org.junit.jupiter:junit-jupiter")
    antlr("org.antlr:antlr4:4.7")
    implementation("org.antlr:antlr4:4.7")
}

tasks.test {
    useJUnitPlatform()
}


tasks.generateGrammarSource {
    arguments = arguments + listOf("-visitor", "-long-messages", "-package", "org.quanta.antlr")
    inputs.dir("src/main/java/org/quanta/antlr")
}

tasks.compileJava {
    dependsOn(tasks.generateGrammarSource)
}

sourceSets {
    main {
        java {
            srcDir("src/main/java")
            srcDir("src/main/antlr")
        }
    }
}