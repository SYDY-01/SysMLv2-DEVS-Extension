import type { SysMLModel } from 'sys-m-lv-2-extended-language';
import { createSysMLv2ExtendedServices, SysMLv2ExtendedLanguageMetaData } from 'sys-m-lv-2-extended-language';
import chalk from 'chalk';
import { Command } from 'commander';
import { extractAstNode } from './util.js';
import { generateJavaScript } from './generator.js';
import { NodeFileSystem } from 'langium/node';
import * as url from 'node:url';
import * as fs from 'node:fs/promises';
import * as path from 'node:path';

const __dirname = url.fileURLToPath(new URL('.', import.meta.url));

const packagePath = path.resolve(__dirname, '..', 'package.json');
const packageContent = await fs.readFile(packagePath, 'utf-8');

export const generateAction = async (fileName: string, opts: GenerateOptions): Promise<void> => {
    const services = createSysMLv2ExtendedServices(NodeFileSystem).SysMLv2Extended;
    
    // 映射修正：使用 8 元组理论前端解析层生成的 SysMLModel 根节点
    const model = await extractAstNode<SysMLModel>(fileName, services);
    
    // 调用生成器 (UR与C算子的占位处)
    const generatedFilePath = generateJavaScript(model, fileName, opts.destination);
    
    console.log(chalk.green(`ASM translation completed successfully: ${generatedFilePath}`));
};

export type GenerateOptions = {
    destination?: string;
}

export default function(): void {
    const program = new Command();

    program.version(JSON.parse(packageContent).version);

    const fileExtensions = SysMLv2ExtendedLanguageMetaData.fileExtensions.join(', ');
    program
        .command('generate')
        .argument('<file>', `source file (possible file extensions: ${fileExtensions})`)
        .option('-d, --destination <dir>', 'destination directory of generating')
        // 更新了架构描述，契合你的论文主题
        .description('Parses SysML v2 extension model and translates it to M2 ASM representation')
        .action(generateAction);

    program.parse(process.argv);
}